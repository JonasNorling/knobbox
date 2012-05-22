#include "TKnobs.h"
#include "TGui.h"
#include "TControllers.h"
#include "device.h"

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

  SwitchData[0] = 0xaa;
  SwitchData[1] = 0xaa;
}

void TKnobs::InitDma()
{
#ifndef HOST
  const uint32_t dma = DMA1;
  const uint32_t txchannel = DMA_CHANNEL4;
  const uint32_t rxchannel = DMA_CHANNEL5;
  DMA_CPAR(dma, rxchannel) = reinterpret_cast<uint32_t>(&USART1_DR); // peripheral addr
  DMA_CMAR(dma, rxchannel) = reinterpret_cast<uint32_t>(SwitchData); // memory addr
  DMA_CPAR(dma, txchannel) = reinterpret_cast<uint32_t>(&USART1_DR); // peripheral addr
  DMA_CMAR(dma, txchannel) = reinterpret_cast<uint32_t>(LedControl); // memory addr
#endif
}

/**
 * \todo This is probably horribly slow -- we need to optimise the DMA
 * channel setup and it's probably not a good idea to make all those
 * library calls that aren't inlined.
 *
 * \todo We shouldn't trigger intterupts for both RX and TX here,
 * that's just stupid.
 */
void TKnobs::StartShifting()
{
  Pin_led_g.Set();

#ifndef HOST
  const uint32_t dma = DMA1;
  const uint32_t txchannel = DMA_CHANNEL4;
  const uint32_t rxchannel = DMA_CHANNEL5;
  //  dma_channel_reset(dma, txchannel);
  //  dma_channel_reset(dma, rxchannel);

  // Disable channels
  DMA_CCR(dma, txchannel) &= ~DMA_CCR_EN;
  DMA_CCR(dma, rxchannel) &= ~DMA_CCR_EN;
  // Reset interrupts
  DMA_IFCR(dma) |= DMA_IFCR_CIF(txchannel) | DMA_IFCR_CIF(rxchannel);
#endif

  /// \todo Think about symmetric PWM to lower the power ripple a bit.
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
#ifndef HOST

  DMA_CCR(dma, rxchannel) = (DMA_CCR_PL_HIGH | // prio
			     DMA_CCR_MSIZE_8BIT |
			     DMA_CCR_PSIZE_8BIT |
			     DMA_CCR_MINC | // memory increment mode
			     DMA_CCR_TCIE // transfer complete interrupt
			     );
  DMA_CNDTR(dma, rxchannel) = ShiftLength; // number of data

  DMA_CCR(dma, txchannel) = (DMA_CCR_PL_LOW | // prio
			     DMA_CCR_MSIZE_8BIT |
			     DMA_CCR_PSIZE_8BIT |
			     DMA_CCR_MINC | // memory increment mode
			     DMA_CCR_DIR // read from memory
			     );
  DMA_CNDTR(dma, txchannel) = ShiftLength; // number of data

  // Enable DMAs
  DMA_CCR(dma, rxchannel) |= DMA_CCR_EN;
  DMA_CCR(dma, txchannel) |= DMA_CCR_EN;
  // Enable RX/TX DMA, kicks off the transfer
  USART_CR3(USART1) |= USART_CR3_DMAR | USART_CR3_DMAT;

#endif

  Pin_led_g.Clear();
}

void TKnobs::Poll()
{
  /// \todo This is probably stupid.

  TEncoderBits data;
  data.EncoderA = ((SwitchData[0] & 0x80) >> 4) |
    ((SwitchData[0] & 0x20) >> 3) |
    ((SwitchData[0] & 0x08) >> 2) |
    ((SwitchData[0] & 0x02) >> 1);
  data.EncoderB = ((SwitchData[0] & 0x40) >> 3) |
    ((SwitchData[0] & 0x10) >> 2) |
    ((SwitchData[0] & 0x04) >> 1) |
    ((SwitchData[0] & 0x01) >> 0);
  data.Button = SwitchData[1];

  const uint16_t edgesA = data.EncoderA ^ LastEncoderData.EncoderA;
  //const uint16_t edgesB = data.EncoderB ^ LastEncoderData.EncoderB;

  if (edgesA) {
    for (int i = 0; i < Knobs; i++) {
      if (edgesA & (1 << i)) {
	if (data.EncoderB & (1 << i)) {
	  Controllers.IncreaseValue(i, 1);
	} else {
	  Controllers.DecreaseValue(i, 1);
	}
      }
      // ... other cases. FIXME: Make this not an if jungle.
    }
  }

  LastEncoderData = data;
}
