#include "device.h"
#include "TDisplay.h"
#include "TGui.h"
#include "TSpiDmaJob.h"
#include "TKnobs.h"
#include "TSequencer.h"
#include "TMidi.h"
#include "TMemory.h"
#include "TControllers.h"
#include "TUsb.h"
#include <new>

/**
 * \file main.cpp
 * \verbatim
 *
 * This code expects to run on a STM32F102C8. 64k flash, 10k RAM.
 * Resources:
 * USART1 - Shift registers for encoders and LEDs, programming
 * USART2 - MIDI in/out
 * USART3 - (unused)
 * SPI1 - LCD and Flash
 * SPI2 - (unused)
 * I2C1 - (unused)
 * I2C1 - (unused)
 * TIM2 - Sequencer event timing, at 48 strikes per quarter note
 * TIM3 -
 * TIM4 -
 * SysTick timer - 1ms master timer
 *
 * Pin connections (specified in device.h):
 *
 * PB15: Vpullup - LCD backlight, encoder pullup voltage
 * BOOT1 (PB2): pulled high
 * BOOT0: connected to SW2, default low
 * LCD display:
 *   SPI1_MOSI (PA7), SPI1_SCK (PA5), PB14: A0, PA4: CS, PA0: RST
 * Atmel SPI flash:
 *   SPI1_MOSI (PA7), SPI1_MISO (PA6), SPI1_SCK (PA5), PB0: CS
 * Buttons:
 *   PB5: SW1, PB4: SW2, PB13: SW3, PB12: SW4
 * MIDI:
 *   USART2_TX (PA2), USART2_RX (PA3)
 * Output shift registers (LED drive):
 *   USART1_TX (PA9): data, USART1_CK (PA8): clock, PB8: load, PB9: OE
 * Input shift registers (Encoder and push button read)
 *   USART1_RX (PA10): data, UART1_CK (PA8): clock, PB6: load, PB7: CE
 *
 * \endverbatim
 */

int Mode;
TDisplay Display;
TSpiDmaQueue SpiDmaQueue;
TGui Gui;
TKnobs Knobs;
TMidi Midi;
TSequencer Sequencer(Midi);
TMemory Memory;
TControllers Controllers;
TUsb Usb;

volatile uint32_t SystemTime = 0; // in ms, wraps at 49.7 days
volatile static uint8_t DmaEvents = 0;
volatile static uint8_t Actions; // Signalling from interrupts to bottom halves
static const uint8_t ACTION_POLL_BUTTONS = 0x01;
static const uint8_t ACTION_BLINK_TIMER = 0x02;


/** DMA channel 1:2 -- SPI1_RX (display and flash) */
void dma1_channel2_isr(void)
{
#ifndef HOST
  dma_disable_transfer_complete_interrupt(DMA1, DMA_CHANNEL2);
  dma_disable_channel(DMA1, DMA_CHANNEL3);
  spi_disable_tx_dma(SPI1);

  // Note: we have to wait until TXE=1 and BSY=0 before changing CS lines.
#endif
  DmaEvents++;
}

/** DMA channel 1:5 -- USART1_RX (shift registers) */
void dma1_channel5_isr(void)
{
#ifndef HOST
  dma_disable_transfer_complete_interrupt(DMA1, DMA_CHANNEL5);
#endif
  /// \todo Once we get the latency in the main loop down, perhaps
  /// this could be moved to a bottom-half?
  Knobs.StartShifting();
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

int main(void)
{
#ifndef HOST
  // Reset some peripherals, this helps when reloading software
  // without issuing a hard reset.
  systick_interrupt_disable();
  systick_counter_disable();
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
#endif

  /// \todo We should wake up in some kind of low power mode.
  clockInit();
  deviceInit();

  Pin_vpullup.Clear();
  Pin_lcd_rst.Clear();
  // Chip selects are active-low.
  Pin_flash_cs.Set();
  Pin_lcd_cs.Set();
  Pin_shift_out_load.Set();
  Pin_shift_out_en.Set();
  Pin_shift_in_en.Set();
  Pin_shift_in_load.Set();

  delay_ms(5);

  // Use placement new to run the constructors of static objects,
  // because libopencm3's crt0 and linker scripts aren't made for C++.
  Mode = MODE_CONTROLLER;
  new(&Display) TDisplay();
  new(&SpiDmaQueue) TSpiDmaQueue();
  new(&Gui) TGui();
  new(&Knobs) TKnobs();
  new(&Midi) TMidi();
  new(&Sequencer) TSequencer(Midi);
  new(&Memory) TMemory();
  new(&Controllers) TControllers();
  new(&Usb) TUsb();

  //Memory.Identify();
  //Memory.ReadStatus();
  Memory.FetchBlock(TMemory::BLOCK_PRODPARAM, 0);

  Usb.Init();

  // Stay quiet for a while to let power stabilise
  delay_ms(150);
  Pin_vpullup.Set();
  delay_ms(150);

  Sequencer.Load();
  Display.Init();

  Knobs.InitDma();
  Knobs.StartShifting();
  Pin_shift_out_en.Clear(); // Turn on LED driver

  while (true) {
    /* Interrupt "bottom half" processing */
#ifndef HOST
    /* We disable the interrupt here to protect DmaEvents, but also
       because it doesn't work otherwise (which is a bit scary). */
    nvic_disable_irq(NVIC_DMA1_CHANNEL2_IRQ);
    if (DmaEvents) {
      DmaEvents--;
      SpiDmaQueue.Finished();
    } else {
      SpiDmaQueue.TryStartJob();
    }
    nvic_enable_irq(NVIC_DMA1_CHANNEL2_IRQ);
#endif

    /* Poll switches */
    if (Actions & ACTION_POLL_BUTTONS) {
      Actions &= ~ACTION_POLL_BUTTONS;

      // Tact switches
      static uint16_t lastState = 0;
      const uint16_t state = gpio_port_read(Pin_sw_1.Port);
      const uint16_t pinMask = Pin_sw_1.Pin | Pin_sw_2.Pin | Pin_sw_3.Pin | Pin_sw_4.Pin;
      if ((state ^ lastState) & pinMask) {
	if (state & Pin_sw_1.Pin) {
	  Gui.Event(KEY_OK);
	} else if (state & Pin_sw_2.Pin) {
	  Gui.Event(KEY_UP);
	} else if (state & Pin_sw_3.Pin) {
	  Gui.Event(KEY_DOWN);
	} else if (state & Pin_sw_4.Pin) {
	  Gui.Event(KEY_BACK);
	}
      }
      lastState = state;

      // Encoders and encoder push buttons
      Knobs.Poll();
    }
    else if (Actions & ACTION_BLINK_TIMER) {
      Actions &= ~ACTION_BLINK_TIMER;
      Gui.Event(BLINK_TIMER);
    }

    Usb.Poll();

    /* Update things */
    Gui.Process();

    /* Sanity check */
#ifndef HOST
    if (DMA_ISR(DMA1) & DMA_ISR_TEIF4) {
      while (true);
    }
    if (DMA_ISR(DMA1) & DMA_ISR_TEIF3) {
      while (true);
    }
    if (DMA_ISR(DMA1) & DMA_ISR_TEIF5) {
      while (true);
    }
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
