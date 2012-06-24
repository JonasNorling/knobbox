#include "TKnobs.h"
#include "TGui.h"
#include "TControllers.h"
#include "TSequencer.h"
#include "device.h"

TKnobs::TKnobs()
{
  LedIntensity[COLOR_RED][0] = 0;
  LedIntensity[COLOR_GREEN][0] = 0;
  LedIntensity[COLOR_RED][1] = 0;
  LedIntensity[COLOR_GREEN][1] = 0;
  LedIntensity[COLOR_RED][2] = 0;
  LedIntensity[COLOR_GREEN][2] = 0;
  LedIntensity[COLOR_RED][3] = 0;
  LedIntensity[COLOR_GREEN][3] = 0;
  CurrentPwmStep = 0;

  SwitchData[0] = 0;
  SwitchData[1] = 0;
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

  // LED driver: clock in the last sent data, always enable outputs.
  Pin_shift_out_load.Set();
  Pin_shift_out_en.Clear();
  Pin_shift_out_load.Clear();

  Pin_shift_in_en.Set();
  Pin_shift_in_load.Clear();
  Pin_shift_in_en.Clear();
  Pin_shift_in_load.Set();

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

  /// \todo Think about symmetric PWM to lower the power ripple a bit.
  // Also, we could scatter the on-state pseudo-randomly in the
  // timeslot, giving a much more even power usage. How will the
  // brightness be affected by that? USe a LUT of some kind?

  // Calculate the next PWM step. It's ok to do this after the DMA job
  // has been started -- we'll output the first byte from the last
  // cycle, but that's good enough for the human eye.
  uint8_t value = 0;
  for (int bit = 0; bit < 8; bit++) {
    const uint8_t color = bit & 0x01;
    const uint8_t knob = bit >> 1;
    value = (value << 1) | (LedIntensity[color][knob] > CurrentPwmStep);
  }
  LedControl[0] = 0;
  LedControl[1] = value;
  CurrentPwmStep++;
}

/*
 * detent  V           V            V
 *   --------.     .-----.     .------.
 * A         |     |     |     |      |
 *           ^-----^     ^-----^      ^-----
 *   -----------.     .-----.     .---------
 * B            |     |     |     |
 *              ^-----^     ^-----^
 *
 */
void TKnobs::Poll()
{
  /// \todo This is probably stupid.

  TEncoderBits data;
  data.EncoderA = ((SwitchData[0] & 0x80) >> 7) |
    ((SwitchData[0] & 0x20) >> 4) |
    ((SwitchData[0] & 0x08) >> 1) |
    ((SwitchData[0] & 0x02) << 2);
  data.EncoderB = ((SwitchData[0] & 0x40) >> 6) |
    ((SwitchData[0] & 0x10) >> 3) |
    ((SwitchData[0] & 0x04) >> 0) |
    ((SwitchData[0] & 0x01) << 3);
  data.Button = SwitchData[1];

  //const uint16_t edgesA = data.EncoderA ^ LastEncoderData.EncoderA;
  const uint16_t edgesB = data.EncoderB ^ LastEncoderData.EncoderB;

  if (edgesB) {
    for (int i = 0; i < Knobs; i++) { // for each encoder
      if (!(data.EncoderA & (1 << i))) { // if A is low
	if (edgesB & (1 << i)) { // B changed

	  /// \todo This should probably be sent through the GUI code,
	  /// and made overall less cryptic.
	  if (Mode == MODE_CONTROLLER) {
	    if (data.EncoderB & (1 << i)) { // B went high
	      Controllers.DecreaseValue(i, 1);
	    } else { // B went low
	      Controllers.IncreaseValue(i, 1);
	    }
	  }
	  else if (Mode == MODE_SEQ) {
	    if (data.EncoderB & (1 << i)) { // B went high
	      Gui.Event(construct_event(KNOB_LEFT, i));
	    } else { // B went low
	      Gui.Event(construct_event(KNOB_RIGHT, i));
	    }
	  }
	}
      }
    }
  }

  LastEncoderData = data;
}
