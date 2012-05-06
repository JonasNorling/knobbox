#include "TKnobs.h"
#include "stm32.h"

TKnobs::TKnobs()
{
  for (int i = 0; i < Knobs; i++) {
    LedIntensity[0][i] = i*(256/Knobs);
    LedIntensity[1][i] = 256 - i*(256/Knobs);
  }
  CurrentPwmStep = 0;
}

void TKnobs::StartShifting()
{
  // FIXME: Think about symmetric PWM to lower the power ripple a bit.
  // Also, we could scatter the on-state pseudo-randomly in the
  // timeslot, giving a much more even power usage. How will the
  // brightness be affected by that? USe a LUT of some kind?

  LedControl[0] = 0;
  // FIXME: bitorder on UART?
  for (int bit=0; bit < 8; bit++) {
    const uint8_t color = bit & 0x01;
    const uint8_t knob = bit >> 1;
    LedControl[1] = (LedControl[1] << 1) |
      (LedIntensity[color][knob] < CurrentPwmStep);
  }

  CurrentPwmStep++;

  // Send LedControl.
  // FIXME: receive SwitchData.
  // FIXME: Don't have to do all this every time.
#ifndef HOST
  const uint32_t dma = DMA1;
  const uint32_t channel = DMA_CHANNEL4;
  dma_channel_reset(dma, channel);
  dma_set_peripheral_address(dma, channel, reinterpret_cast<uint32_t>(&USART1_DR));
  dma_set_memory_address(dma, channel, reinterpret_cast<uint32_t>(LedControl));
  dma_set_number_of_data(dma, channel, sizeof(LedControl));
  dma_set_read_from_memory(dma, channel);
  dma_enable_memory_increment_mode(dma, channel);
  dma_set_peripheral_size(dma, channel, DMA_CCR_PSIZE_8BIT);
  dma_set_memory_size(dma, channel, DMA_CCR_MSIZE_8BIT);
  dma_set_priority(dma, channel, DMA_CCR_PL_LOW);
  dma_enable_transfer_complete_interrupt(dma, channel);
  dma_enable_channel(dma, channel);
  usart_enable_tx_dma(USART1);
#endif
}
