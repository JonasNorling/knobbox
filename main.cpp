#include "device.h"
#include "TDisplay.h"
#include "TGui.h"
#include "TSpiDmaJob.h"
#include "TKnobs.h"
#include "TSwitches.h"
#include "TSequencer.h"
#include "TMidi.h"
#include "TMidiParser.h"
#include "TMemory.h"
#include "TControllers.h"
#include "TUsb.h"
#include <new>
#include "TScheduler.h"
#include "TLeds.h"
#include "TFlash.h"

/**
 * \file main.cpp
 * \verbatim
 *
 * This code expects to run on a STM32F103RC. 256k flash, 48k RAM.
 * Resources:
 * USART1 - Shift registers for encoders and LEDs
 * USART2 - MIDI 1 in/out
 * USART3 - MIDI 2 in/out
 * SPI1 - AT45 Flash
 * SPI2 - LCD
 * TIM2 - Sequencer event timing, at 48 strikes per quarter note
 * TIM3 -
 * TIM4 -
 * SysTick timer - 1ms master timer
 *
 * \endverbatim
 */

int Mode;
TDisplay Display;
TSpiDmaQueue SpiDmaQueue;
TGui Gui;
TKnobs Knobs;
TMidi Midi;
TMidiParser MidiParser;
TSequencer Sequencer(Midi);
TSwitches Switches;
TMemory Memory;
TFlash Flash;
TControllers Controllers(Midi);
TUsb Usb;

static void gui_task(void) __attribute__((noreturn));
static void flash_task(void) __attribute__((noreturn));
static uint8_t __attribute__((aligned(8))) gui_task_stack[320];
static uint8_t __attribute__((aligned(8))) flash_task_stack[320];
TScheduler::TTaskControlBlock TScheduler::Tcbs[SCHEDULER_NUM_TASKS];
uint8_t TScheduler::CurrentTask;
const TScheduler::TTask TScheduler::Tasks[SCHEDULER_NUM_TASKS] = {
        { "main", 0, 0, 0 },
        { "gui", gui_task, gui_task_stack, sizeof(gui_task_stack) },
        { "flash", flash_task, flash_task_stack, sizeof(flash_task_stack) },
};

enum {
    SPI_DMA_STATE_IDLE = 0,
    SPI_DMA_STATE_RUNNING,
    SPI_DMA_STATE_FINISHED
};

volatile uint32_t SystemTime = 0; // in ms, wraps at 49.7 days
volatile static uint8_t SpiDmaState = SPI_DMA_STATE_IDLE;
volatile static uint8_t Actions; // Signaling from interrupts to bottom halves
static const uint8_t ACTION_POLL_BUTTONS = 0x01;
static const uint8_t ACTION_BLINK_TIMER = 0x02;

/* Spinning fault handlers, so gdb tells us what we got. */

#ifndef HOST
#include "libopencm3/include/libopencm3/stm32/f1/scb.h"
void hard_fault_handler(void)
{
    const volatile __attribute__((unused)) uint32_t* cfsr = &SCB_CFSR;
    const volatile __attribute__((unused)) uint32_t* hfsr = &SCB_HFSR;
    const volatile __attribute__((unused)) uint32_t* bfar = &SCB_BFAR;
    TLeds::Set(TLeds::LED_ENC_GREEN, false);
    while (1) {
        TLeds::Set(TLeds::LED_TP9, false);
        TLeds::Set(TLeds::LED_TP16, false);
        TLeds::Set(TLeds::LED_ENC_RED, false);
        delay_ms(1000);

        TLeds::Set(TLeds::LED_TP9, true);
        TLeds::Set(TLeds::LED_TP16, true);
        TLeds::Set(TLeds::LED_ENC_RED, true);
        delay_ms(1000);
    }
}
#endif

void mem_manage_handler(void)
{
    while (1);
}

void bus_fault_handler(void)
{
    while (1);
}

void usage_fault_handler(void)
{
    while (1);
}

/** DMA channel 1:2 -- SPI1_RX (flash) */
void dma1_channel2_isr(void)
{
#ifndef HOST
    dma_disable_transfer_complete_interrupt(DMA1, FLASH_DMA_RX_CHANNEL);
    dma_disable_channel(DMA1, FLASH_DMA_TX_CHANNEL);
    spi_disable_tx_dma(FLASH_SPI_CHANNEL);

    // Note: we have to wait until TXE=1 and BSY=0 before changing CS lines.
#endif

    TBlockingSpiDmaJob::FinishedFromIsr();
}

/** DMA channel 1:4 -- SPI2_RX (display) */
void dma1_channel4_isr(void)
{
#ifndef HOST
    dma_disable_transfer_complete_interrupt(DMA1, LCD_DMA_RX_CHANNEL);
    dma_disable_channel(DMA1, LCD_DMA_TX_CHANNEL);
    spi_disable_tx_dma(LCD_SPI_CHANNEL);

    // Note: we have to wait until TXE=1 and BSY=0 before changing CS lines.
#endif
    assert(SpiDmaState == SPI_DMA_STATE_RUNNING);

    SpiDmaState = SPI_DMA_STATE_FINISHED;
}

void usart2_isr(void)
{
    Midi.GotInterrupt();
}

/** ARM systick timer: millisecond counter */
void sys_tick_handler(void)
{
    SystemTime++;
    if (!(SystemTime % 1000)) {
    }
    if (!(SystemTime % 128)) {
        Actions |= ACTION_BLINK_TIMER;
    }
    Actions |= ACTION_POLL_BUTTONS;
}

/** TIM2: sequencer 48th beat counter */
void tim2_isr(void)
{
#ifndef HOST
    TIM_SR(TIM2) &= ~TIM_SR_UIF; // Clear interrupt
#endif
    Sequencer.Step();
}

static void gui_task(void)
{
    Gui.Show();
}

static void flash_task(void)
{
    Flash.FlashTask();
}

void pollSpiDma()
{
#ifndef HOST
    /* We disable the interrupt here to protect SpiDmaState, but also
     * because it doesn't work otherwise (which is a bit scary).
     * FIXME: SpiDmaState shouldn't need any protection, because writes
     * to it are atomic?
     */
    nvic_disable_irq(NVIC_DMA1_CHANNEL4_IRQ);
    switch (SpiDmaState) {
    case SPI_DMA_STATE_RUNNING:
        break;
    case SPI_DMA_STATE_FINISHED:
        SpiDmaQueue.Finished();
        SpiDmaState = SPI_DMA_STATE_IDLE;
        break;
    case SPI_DMA_STATE_IDLE:
        if (SpiDmaQueue.TryStartJob()) {
            SpiDmaState = SPI_DMA_STATE_RUNNING;
        }
        break;
    }
    nvic_enable_irq(NVIC_DMA1_CHANNEL4_IRQ);
#endif
}

int main(void)
{
    clockInit();

#ifndef HOST
    // Reset some peripherals, this helps when reloading software
    // without issuing a hard reset.
    nvic_disable_irq(NVIC_DMA1_CHANNEL2_IRQ);
    nvic_disable_irq(NVIC_DMA1_CHANNEL3_IRQ);
    nvic_disable_irq(NVIC_DMA1_CHANNEL4_IRQ);
    nvic_disable_irq(NVIC_DMA1_CHANNEL5_IRQ);
    nvic_disable_irq(NVIC_TIM2_IRQ);
    const uint32_t resets2 =
            RCC_APB2RSTR_USART1RST |
            RCC_APB2RSTR_SPI1RST;
    const uint32_t resets1 =
            RCC_APB1RSTR_USART2RST;
    rcc_peripheral_reset(&RCC_APB2RSTR, resets2);
    rcc_peripheral_reset(&RCC_APB1RSTR, resets1);
    rcc_peripheral_clear_reset(&RCC_APB2RSTR, resets2);
    rcc_peripheral_clear_reset(&RCC_APB1RSTR, resets1);
    DMA_IFCR(DMA1) = 0x0fffffff; // Clear pending DMA interrupts

    nvic_clear_pending_irq(NVIC_DMA1_CHANNEL2_IRQ);
    nvic_clear_pending_irq(NVIC_DMA1_CHANNEL3_IRQ);
    nvic_clear_pending_irq(NVIC_DMA1_CHANNEL4_IRQ);
    nvic_clear_pending_irq(NVIC_DMA1_CHANNEL5_IRQ);

    assert(!(USART_SR(USART1) & USART_SR_RXNE));
    assert(!nvic_get_pending_irq(NVIC_DMA1_CHANNEL5_IRQ));
#endif

    deviceInit();

    TLeds::Set(TLeds::LED_TP9, false);
    TLeds::Set(TLeds::LED_TP16, false);
    TLeds::Set(TLeds::LED_ENC_GREEN, false);
    TLeds::Set(TLeds::LED_ENC_RED, false);
    TLeds::Set(TLeds::LED_1, false);
    TLeds::Set(TLeds::LED_2, false);
    TLeds::Set(TLeds::LED_3, false);
    TLeds::Set(TLeds::LED_4, false);
    TLeds::Set(TLeds::LED_LCD_BL, false);

    Pin_vpullup.Clear();
    // Chip selects are active-low.
    Pin_flash_cs.Set();
    Pin_lcd_cs.Set();
    Pin_shift_out_load.Set();
    Pin_shift_out_en.Set();
    Pin_shift_in_en.Set();
    Pin_shift_in_load.Set();

    // Use placement new to run the constructors of static objects,
    // because libopencm3's crt0 and linker scripts aren't made for C++.
    Mode = MODE_CONTROLLER;
    new(&Display) TDisplay();
    new(&SpiDmaQueue) TSpiDmaQueue();
    new(&Gui) TGui();
    new(&Knobs) TKnobs();
    new(&Switches) TSwitches();
    new(&Midi) TMidi();
    new(&MidiParser) TMidiParser();
    new(&Sequencer) TSequencer(Midi);
    new(&Memory) TMemory();
    new(&Flash) TFlash();
    new(&Controllers) TControllers(Midi);
    new(&Usb) TUsb();

    Usb.Init();

    // Stay quiet for a while to let power stabilise
    delay_ms(150);
    Pin_vpullup.Set();
    delay_ms(150);

    Sequencer.Load();
    Display.Init();

    Knobs.StartShifting();
    Pin_shift_out_en.Clear(); // Turn on LED driver
    Pin_shift_in_en.Clear();

    TScheduler::Init();
    while (!Flash.Inited()) {
        TScheduler::Yield();
    }

    Midi.EnableRxInterrupt();

    TLeds::Set(TLeds::LED_ENC_GREEN, true);
    TLeds::Set(TLeds::LED_LCD_BL, true);

    while (true) {
        /* Interrupt "bottom half" processing */
        pollSpiDma();

        if (Actions & ACTION_POLL_BUTTONS) {
            Actions &= ~ACTION_POLL_BUTTONS;

            // Tact switches and the central encoder
            Switches.Poll();

            // Encoders and encoder push buttons
            Knobs.Poll();
        }
        else if (Actions & ACTION_BLINK_TIMER) {
            Actions &= ~ACTION_BLINK_TIMER;
            Gui.Event(BLINK_TIMER);
        }

        /* Poll I/O */
        Usb.Poll();
        if (!Midi.InQueue.Empty()) {
            uint8_t b = Midi.InQueue.First();
            Midi.InQueue.Remove();

            // Feed through MIDI data
            Midi.EnqueueByte(b);

            TMidiParser::ParseResult res = MidiParser.Feed(b);
            if (res == TMidiParser::GOT_EVENT) {
                const TMidiEvent& event = MidiParser.GetEvent();
                if (event.GetType() == TMidiEvent::MIDI_CC) {
                    Controllers.MidiEvent(event);
                }
                else {
                    Sequencer.MidiEvent(event);
                }
            }
            else if (res == TMidiParser::GOT_REALTIME) {
                Sequencer.MidiRealtimeMessage(MidiParser.GetRealtimeMessage());
            }
        }

        /* Run other tasks */
        TScheduler::Yield();

#ifndef HOST
        /* Sanity check */
        assert(!(DMA_ISR(DMA1) & DMA_ISR_TEIF4));
        assert(!(DMA_ISR(DMA1) & DMA_ISR_TEIF3));
        assert(!(DMA_ISR(DMA1) & DMA_ISR_TEIF5));
#endif

#ifdef HOST
        static int counter = 0;
        if (counter++ > 1000) {
            Display.DumpPixels();
            return 0;
        }
#endif
    }

    return 0;
}
