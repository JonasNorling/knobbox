#include "stm32.h"
#include "TDisplay.h"
#include "TGui.h"
#include "TSpiDmaJob.h"

TDisplay Display;
TSpiDmaQueue SpiDmaQueue;
TGui Gui;

void gpioInit()
{
#ifndef HOST
  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN);
  // LCD A0
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO14);
#endif
}

void spiInit()
{
#ifndef HOST
  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_SPI1EN);
  spi_set_baudrate_prescaler(SPI1, SPI_CR1_BR_FPCLK_DIV_256);
#endif
}

volatile static uint8_t DmaEvents = 0;
void dma1_channel7_isr(void)
{
  DmaEvents++;
}

int main(void)
{
#ifndef HOST
  // FIXME: Can't run this fast?
  rcc_clock_setup_in_hse_8mhz_out_72mhz();
#endif

  // FIXME: We should wake up in some kind of low power mode.
  gpioInit();
  spiInit();

  Display.Init();
  Display.Power(true);

  while (true) {
    if (DmaEvents) {
      DmaEvents--;
      SpiDmaQueue.Finished();
    } else {
      return 0;
    }
    Gui.Process();
  }

  return 0;
}
