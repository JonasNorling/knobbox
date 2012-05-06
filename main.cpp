#include "stm32.h"
#include "TDisplay.h"
#include "TGui.h"
#include "TSpiDmaJob.h"
#include <new>

TDisplay Display;
TSpiDmaQueue SpiDmaQueue;
TGui Gui;

/*
 * This code expects to run on a STM32F101C8.
 * Reosurces:
 * USART1 - Shift registers for encoders and LEDs, programming
 * USART2 - MIDI in/out
 * USART3 - (unused)
 * SPI1 - LCD and Flash
 * SPI2 - (unused)
 * I2C1 - (unused)
 * I2C1 - (unused)
 * TIM2 -
 * TIM3 -
 * TIM4 -
 * SysTick timer - 0.5ms master timer, sequencer tick
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

void clockInit()
{
#ifndef HOST
  // FIXME: Want to run at 48 MHz.
  rcc_clock_setup_in_hse_8mhz_out_24mhz();

  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN); // GPIOA
  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN); // GPIOB
  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPCEN); // GPIOC

  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_SPI1EN); // SPI1
  rcc_peripheral_enable_clock(&RCC_AHBENR, RCC_AHBENR_DMA1EN); // DMA1
#endif
}


void gpioInit()
{
#ifndef HOST
  /* There is some space to save here:
   *  - gcc refuses to inline SetOutput randomly
   *  - pin configurations in the same port can be ored together
   */

  Pin_lcd_a0.SetOutput();
  Pin_lcd_cs.SetOutput();
  Pin_lcd_rst.SetOutput();

  // Discovery: LEDs on PC8 and PC9
  Pin_led_b.SetOutput(GPIO_MODE_OUTPUT_2_MHZ);
  Pin_led_g.SetOutput(GPIO_MODE_OUTPUT_2_MHZ);

  // Debug LEDs (Discovery)
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5);

  // SPI
  Pin_spi_mosi.SetOutput(GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL);
  //TPin::SetOutput(Pin_spi_miso, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL);
  Pin_spi_sck.SetOutput(GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL);
#endif
}

void spiInit()
{
#ifndef HOST
  spi_set_unidirectional_mode(SPI1);
  spi_disable_crc(SPI1);
  spi_set_dff_8bit(SPI1);
  spi_set_full_duplex_mode(SPI1);
  spi_enable_software_slave_management(SPI1);
  spi_set_nss_high(SPI1);
  spi_set_baudrate_prescaler(SPI1, SPI_CR1_BR_FPCLK_DIV_256);
  spi_set_master_mode(SPI1);
  spi_set_clock_polarity_1(SPI1);
  spi_set_clock_phase_1(SPI1);
  spi_enable(SPI1);

  // Enable SPI TX DMA IRQ.
  nvic_set_priority(NVIC_DMA1_CHANNEL3_IRQ, 0);
  nvic_enable_irq(NVIC_DMA1_CHANNEL3_IRQ);
#endif
}

/* DMA channel 1:7 -- USART2_TX */
void dma1_channel7_isr(void);

/* DMA channel 1:6 -- USART2_RX */
void dma1_channel6_isr(void);

/* DMA channel 1:5 -- USART1_RX */
void dma1_channel5_isr(void);

/* DMA channel 1:4 -- USART1_TX */
void dma1_channel4_isr(void);

/* DMA channel 1:3 -- SPI1_TX */
volatile static uint8_t DmaEvents = 0;
void dma1_channel3_isr(void)
{
#ifndef HOST
  Pin_led_b.Toggle();

  dma_disable_transfer_complete_interrupt(DMA1, DMA_CHANNEL3);
  dma_disable_channel(DMA1, DMA_CHANNEL3);
  spi_disable_tx_dma(SPI1);

  DmaEvents++;
  GPIO_ODR(GPIOC)++; // Debug LEDs

  // Note: we have to wait until TXE=1 and BSY=0 before changing CS lines.
#endif
}

/* DMA channel 1:2 -- SPI1_RX */
void dma1_channel2_isr(void);

int main(void)
{
  // Use placement new to run the constructors of static objects,
  // because libopencm3's crt0 and linker scripts aren't made for C++.
  new(&Display) TDisplay();
  new(&SpiDmaQueue) TSpiDmaQueue();
  new(&Gui) TGui();

  // FIXME: We should wake up in some kind of low power mode.
  clockInit();
  gpioInit();
  spiInit();

  delay_ms(5);
  Display.Init();
  //Display.Power(true);

  while (true) {
    delay_ms(5);
#ifndef HOST
    Pin_led_g.Toggle();
#endif
    if (DmaEvents) {
      DmaEvents--;
      SpiDmaQueue.Finished();
    } else {
      SpiDmaQueue.TryStartJob();
#ifdef HOST
      Display.DumpPixels();
      return 0;
#endif
    }
    Gui.Process();
  }

  return 0;
}
