#include "device.h"
#include "TDisplay.h"
#include "TGui.h"
#include "TSpiDmaJob.h"
#include "TKnobs.h"
#include "TSequencer.h"
#include "TMidi.h"
#include "TMidiParser.h"
#include "TMemory.h"
#include "TControllers.h"
#include "TUsb.h"
#include <new>
#include "TScheduler.h"

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
TMemory Memory;
TControllers Controllers(Midi);
TUsb Usb;

static void gui_task(void) __attribute__((noreturn));
static uint8_t __attribute__((aligned(8))) gui_task_stack[320];
TScheduler::TTaskControlBlock TScheduler::Tcbs[SCHEDULER_NUM_TASKS];
uint8_t TScheduler::CurrentTask;
const TScheduler::TTask TScheduler::Tasks[SCHEDULER_NUM_TASKS] = {
        { "main", 0, 0, 0 },
        { "gui", gui_task, gui_task_stack, sizeof(gui_task_stack) },
};

enum {
    SPI_DMA_STATE_IDLE = 0,
    SPI_DMA_STATE_RUNNING,
    SPI_DMA_STATE_FINISHED
};

volatile uint32_t SystemTime = 0; // in ms, wraps at 49.7 days
volatile static uint8_t SpiDmaState = SPI_DMA_STATE_IDLE;
volatile static uint8_t Actions; // Signalling from interrupts to bottom halves
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
    while (1) {
        Pin_vpullup.Clear();
        delay_ms(1000);
        Pin_vpullup.Set();
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
    assert(SpiDmaState == SPI_DMA_STATE_RUNNING);

    SpiDmaState = SPI_DMA_STATE_FINISHED;
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
    Pin_led_tp16.Toggle();
    Sequencer.Step();
}

static void gui_task(void)
{
    Gui.Show();
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

    Pin_lcd_backlight.Clear();
    Pin_led_tp9.Set(); // Turn on LED
    Pin_led_tp16.Set(); // Turn on LED
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
    new(&Midi) TMidi();
    new(&MidiParser) TMidiParser();
    new(&Sequencer) TSequencer(Midi);
    new(&Memory) TMemory();
    new(&Controllers) TControllers(Midi);
    new(&Usb) TUsb();

    //Memory.FetchBlock(TMemory::BLOCK_PRODPARAM, 0);

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
    TScheduler::Yield();

    Midi.EnableRxInterrupt();

    Pin_led_tp9.Clear(); // Turn off LED
    Pin_led_tp16.Clear(); // Turn off LED
    Pin_led_enc1.Clear();
    Pin_led_enc1.Set();
    Pin_led_1.Set();
    Pin_led_2.Set();
    Pin_led_3.Clear();
    Pin_led_4.Clear();
    Pin_lcd_backlight.Set();

    while (true) {
        /* Interrupt "bottom half" processing */
#ifndef HOST
        /* We disable the interrupt here to protect SpiDmaState, but also
         * because it doesn't work otherwise (which is a bit scary). */
        nvic_disable_irq(NVIC_DMA1_CHANNEL2_IRQ);
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
        nvic_enable_irq(NVIC_DMA1_CHANNEL2_IRQ);
        nvic_enable_irq(NVIC_DMA1_CHANNEL4_IRQ);
#endif

        /* Poll switches */
        if (Actions & ACTION_POLL_BUTTONS) {
            Actions &= ~ACTION_POLL_BUTTONS;

            // Tact switches
            static uint8_t lastState = 0;
            static uint32_t pressTime[4] = { 0 };
            const uint16_t port = gpio_port_read(Pin_sw_1.Port);
            const uint8_t state =
                    ((port & Pin_sw_1.Pin) ? 0x01 : 0x00) |
                    ((port & Pin_sw_2.Pin) ? 0x02 : 0x00) |
                    ((port & Pin_sw_3.Pin) ? 0x04 : 0x00) |
                    ((port & Pin_sw_4.Pin) ? 0x08 : 0x00);

            if (state ^ lastState) {
                // At least one button has changed state

                const uint16_t pressed = state & ~lastState;
                const uint16_t released = lastState & ~state;

                for (int i = 0; i < 4; i++) {
                    if (pressed & (1 << i)) {
                        pressTime[i] = SystemTime;
                        Gui.Event(KEY_OK + 0x10 * i);
                    }
                    if (released & (1 << i)) {
                        pressTime[i] = 0;
                    }
                }
            }
            lastState = state;

            const int LONG_PRESS_DELAY_MS = 1000;
            for (int i = 0; i < 4; i++) {
                if (pressTime[i] != 0 && SystemTime > (pressTime[i] + LONG_PRESS_DELAY_MS)) {
                    Gui.Event(KEY_LONGPRESS_OK + 0x10 * i);
                    pressTime[i] = 0;
                }
            }

            // Encoders and encoder push buttons
            Knobs.Poll();
        }
        else if (Actions & ACTION_BLINK_TIMER) {
            Actions &= ~ACTION_BLINK_TIMER;
            Gui.Event(BLINK_TIMER);
            //Pin_led_tp9.Toggle(); // Blink LED
            //Pin_led_tp16.Toggle(); // Blink LED
            Pin_led_4.Toggle(); // Blink LED
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

        /* Run GUI task */
        if (Gui.EventIsPending() || Gui.HaveDirtyLines()) {
            TScheduler::Yield();
        }

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
