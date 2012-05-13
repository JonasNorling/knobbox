#include "TKnobs.h"
#include "stm32.h"

TKnobs::TKnobs()
{
  /*
  for (int i = 0; i < Knobs; i++) {
    LedIntensity[0][i] = i*(256/Knobs);
    LedIntensity[1][i] = 256 - i*(256/Knobs);
  }
  */
  LedIntensity[0][0] = 255;
  LedIntensity[1][0] = 0;
  LedIntensity[0][1] = 100;
  LedIntensity[1][1] = 0;
  LedIntensity[0][2] = 50;
  LedIntensity[1][2] = 0;
  LedIntensity[0][3] = 5;
  LedIntensity[1][3] = 0;
  CurrentPwmStep = 0;
}

void TKnobs::StartShifting()
{
#ifndef HOST
  const uint32_t dma = DMA1;
  const uint32_t txchannel = DMA_CHANNEL4;
  const uint32_t rxchannel = DMA_CHANNEL5;
  dma_channel_reset(dma, txchannel);
  dma_channel_reset(dma, rxchannel);
#endif

  // FIXME: Think about symmetric PWM to lower the power ripple a bit.
  // Also, we could scatter the on-state pseudo-randomly in the
  // timeslot, giving a much more even power usage. How will the
  // brightness be affected by that? USe a LUT of some kind?

  LedControl[0] = 0;
  for (int bit=0; bit < 8; bit++) {
    const uint8_t color = bit & 0x01;
    const uint8_t knob = bit >> 1;
    LedControl[1] = (LedControl[1] << 1) |
      (LedIntensity[color][knob] > CurrentPwmStep);
  }

  CurrentPwmStep++;

  // Send LedControl.
  // FIXME: receive SwitchData.
#ifndef HOST
  dma_set_peripheral_address(dma, txchannel, reinterpret_cast<uint32_t>(&USART1_DR));
  dma_set_memory_address(dma, txchannel, reinterpret_cast<uint32_t>(LedControl));
  dma_set_number_of_data(dma, txchannel, ShiftLength);
  dma_set_read_from_memory(dma, txchannel);
  dma_enable_memory_increment_mode(dma, txchannel);
  dma_set_peripheral_size(dma, txchannel, DMA_CCR_PSIZE_8BIT);
  dma_set_memory_size(dma, txchannel, DMA_CCR_MSIZE_8BIT);
  dma_set_priority(dma, txchannel, DMA_CCR_PL_LOW);
  dma_enable_transfer_complete_interrupt(dma, txchannel);
  dma_enable_channel(dma, txchannel);
  usart_enable_tx_dma(USART1);
  // Will trigger dma_channel4_isr when done.

  /*
  dma_set_peripheral_address(dma, rxchannel, reinterpret_cast<uint32_t>(&USART1_DR));
  dma_set_memory_address(dma, rxchannel, reinterpret_cast<uint32_t>(LedControl));
  dma_set_number_of_data(dma, rxchannel, ShiftLength);
  dma_set_read_from_peripheral(dma, rxchannel);
  dma_enable_memory_increment_mode(dma, rxchannel);
  dma_set_peripheral_size(dma, rxchannel, DMA_CCR_PSIZE_8BIT);
  dma_set_memory_size(dma, rxchannel, DMA_CCR_MSIZE_8BIT);
  dma_set_priority(dma, rxchannel, DMA_CCR_PL_HIGH);
  dma_enable_transfer_complete_interrupt(dma, rxchannel);
  dma_enable_channel(dma, rxchannel);
  usart_enable_rx_dma(USART1);
  */
  // Will trigger dma_channel5_isr when done.

#endif
}
