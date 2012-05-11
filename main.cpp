#include "stm32.h"
#include "TDisplay.h"
#include "TGui.h"
#include "TSpiDmaJob.h"
#include "TKnobs.h"
#include <new>

/*
 * This code expects to run on a STM32F101C8.
 * Resources:
 * USART1 - Shift registers for encoders and LEDs, programming
 * USART2 - MIDI in/out
 * USART3 - (unused)
 * SPI1 - LCD and Flash
 * SPI2 - (unused)
 * I2C1 - (unused)
 * I2C1 - (unused)
 * TIM2 - Sequencer event timing?
 * TIM3 -
 * TIM4 -
 * SysTick timer - 1ms master timer
 *
 * Pin connections (specified in stm32.h):
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
 */

TDisplay Display;
TSpiDmaQueue SpiDmaQueue;
TGui Gui;
TKnobs Knobs;
uint32_t SystemTime = 0; // in ms, wraps at 49.7 days

/* DMA channel 1:3 -- SPI1_TX (display and flash) */
volatile static uint8_t DmaEvents = 0;
void dma1_channel3_isr(void)
{
#ifndef HOST
  dma_disable_transfer_complete_interrupt(DMA1, DMA_CHANNEL3);
  dma_disable_channel(DMA1, DMA_CHANNEL3);
  spi_disable_tx_dma(SPI1);

  // Note: we have to wait until TXE=1 and BSY=0 before changing CS lines.
#endif
  DmaEvents++;
}

/* DMA channel 1:4 -- USART1_TX (shift registers) */
void dma1_channel4_isr(void)
{
#ifndef HOST
  dma_disable_transfer_complete_interrupt(DMA1, DMA_CHANNEL4);
  Pin_led_b.Toggle();
#endif
  // FIXME: Once we get the latency in the main loop down, perhaps
  // this could be moved to a bottom-half?
  Knobs.StartShifting();
}

/* DMA channel 1:5 -- USART1_RX (shift registers) */
void dma1_channel5_isr(void)
{
#ifndef HOST
  dma_disable_transfer_complete_interrupt(DMA1, DMA_CHANNEL5);
#endif
}

void sys_tick_handler(void)
{
#ifndef HOST
  SystemTime++;
  if (!(SystemTime % 1000)) {
    Pin_led_g.Toggle();
  }
#endif  
}


int main(void)
{
  // Use placement new to run the constructors of static objects,
  // because libopencm3's crt0 and linker scripts aren't made for C++.
  new(&Display) TDisplay();
  new(&SpiDmaQueue) TSpiDmaQueue();
  new(&Gui) TGui();
  new(&Knobs) TKnobs();

  // FIXME: We should wake up in some kind of low power mode.
  clockInit();
  deviceInit();

  delay_ms(5);
  Display.Init();
  //Display.Power(true);

  Knobs.StartShifting();

  while (true) {
    /* Interrupt "bottom half" processing */
    if (DmaEvents) {
      // FIXME: race condition? Is there a test-and-decrement instruction?
      DmaEvents--;
      SpiDmaQueue.Finished();
    } else {
      SpiDmaQueue.TryStartJob();
#ifdef HOST
      Display.DumpPixels();
      return 0;
#endif
    }

    /* Poll switches */
#ifndef HOST
    Pin_led_g.Toggle();
    {
      // FIXME: Poll more seldom (each ms or something), it's a cheap
      // way to ignore bounces and spurious triggers when releasing
      // the button.
      for (unsigned i=0; i < 1000; i++) __asm__("nop");

      static uint16_t lastState = 0;
      const uint16_t state = gpio_port_read(Pin_sw_1.Port);
      const uint16_t pinMask = Pin_sw_1.Pin | Pin_sw_2.Pin | Pin_sw_3.Pin | Pin_sw_4.Pin;
      if ((state ^ lastState) & pinMask) {
	if (~state & Pin_sw_1.Pin) {
	  Gui.KeyEvent(KEY_OK);
	} else if (~state & Pin_sw_2.Pin) {
	  Gui.KeyEvent(KEY_UP);
	} else if (~state & Pin_sw_3.Pin) {
	  Gui.KeyEvent(KEY_DOWN);
	} else if (~state & Pin_sw_4.Pin) {
	  Gui.KeyEvent(KEY_BACK);
	}
      }
      lastState = state;
    }
#endif

    /* Update things */
    Gui.Process();

    /* Sanity check */
#ifndef HOST
    if (DMA_ISR(DMA1) & DMA_ISR_TEIF4) {
      gpio_toggle(GPIOC, GPIO0);
    }
    if (DMA_ISR(DMA1) & DMA_ISR_TEIF3) {
      gpio_toggle(GPIOC, GPIO1);
    }
    if (DMA_ISR(DMA1) & DMA_ISR_TEIF5) {
      gpio_toggle(GPIOC, GPIO2);
    }
#endif
  }

  return 0;
}
