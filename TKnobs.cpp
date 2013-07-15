#include "TKnobs.h"
#include "TGui.h"
#include "TControllers.h"
#include "TSequencer.h"
#include "TLeds.h"
#include "device.h"

static const uint32_t USART = USART1;

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

#ifndef HOST
static uint8_t txbyteno = 0;
#endif

void usart1_isr(void)
{
#ifndef HOST
    TLeds::Set(TLeds::LED_TP9, true);
    //assert(!(USART_SR(USART) & USART_SR_ORE));
    //assert(USART_SR(USART) & USART_SR_TC);
    //assert(USART_SR(USART) & USART_SR_RXNE);

    //while (USART_SR(USART) & USART_SR_RXNE)
    {
        assert(txbyteno <= Knobs.ShiftLength);
        Knobs.SwitchData[txbyteno - 1] = USART_DR(USART);
    }

    if (txbyteno >= Knobs.ShiftLength) {
        // All bytes have really been sent now
        txbyteno = 0;

        // Latch the LED data.
        // Knob input: sample encoder positions and push buttons
        Pin_shift_out_load.Set();
        Pin_shift_in_load.Clear();
        Pin_shift_out_load.Clear();
        Pin_shift_in_load.Set();

        // Send the first byte before recalculating PWM, speeding up
        // matters a bit.
        USART_DR(USART) = Knobs.LedControl[txbyteno++];

        // Calculate the next PWM step.
        Knobs.LedControl[0] = 0;
        Knobs.LedControl[1] = 0;
        for (int byte = 0; byte < 4; byte++) {
            uint8_t value = 0;
            for (int bit = 0; bit < 8; bit++) {
                const uint8_t color = bit & 0x01;
                const uint8_t knob = (4 * byte) + (bit >> 1);
                value = (value << 1) | (Knobs.LedIntensity[color][knob] > Knobs.CurrentPwmStep);
            }
            Knobs.LedControl[5 - byte] = value;
        }
        Knobs.CurrentPwmStep += 127;
    }
    else {
        USART_DR(USART) = Knobs.LedControl[txbyteno++];
    }

    TLeds::Set(TLeds::LED_TP9, false);
#endif
}

void TKnobs::StartShifting()
{
#ifndef HOST
    txbyteno++;
    USART_DR(USART) = 0; // Send something
    //USART_CR1(USART) |= USART_CR1_TCIE; // Enable TX complete interrupt
    //USART_CR1(USART) |= USART_CR1_TXEIE; // Enable TX interrupt
    USART_CR1(USART) |= USART_CR1_RXNEIE; // Enable RX interrupt
#endif
}

#define BIT(a, n) (((a) & (1 << (n))) >> (n))

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
  // FIXME: This is probably stupid.

  TEncoderBits data;
  data.EncoderA =
          (BIT(SwitchData[0], 7) << 0) |
          (BIT(SwitchData[0], 5) << 1) |
          (BIT(SwitchData[0], 3) << 2) |
          (BIT(SwitchData[0], 1) << 3) |
          (BIT(SwitchData[2], 7) << 4) |
          (BIT(SwitchData[2], 5) << 5) |
          (BIT(SwitchData[2], 3) << 6) |
          (BIT(SwitchData[2], 1) << 7) |
          (BIT(SwitchData[3], 7) << 8) |
          (BIT(SwitchData[3], 5) << 9) |
          (BIT(SwitchData[3], 3) <<10) |
          (BIT(SwitchData[3], 1) <<11) |
          (BIT(SwitchData[5], 7) <<12) |
          (BIT(SwitchData[5], 5) <<13) |
          (BIT(SwitchData[5], 3) <<14) |
          (BIT(SwitchData[5], 1) <<15);
  data.EncoderB =
          (BIT(SwitchData[0], 6) << 0) |
          (BIT(SwitchData[0], 4) << 1) |
          (BIT(SwitchData[0], 2) << 2) |
          (BIT(SwitchData[0], 0) << 3) |
          (BIT(SwitchData[2], 6) << 4) |
          (BIT(SwitchData[2], 4) << 5) |
          (BIT(SwitchData[2], 2) << 6) |
          (BIT(SwitchData[2], 0) << 7) |
          (BIT(SwitchData[3], 6) << 8) |
          (BIT(SwitchData[3], 4) << 9) |
          (BIT(SwitchData[3], 2) <<10) |
          (BIT(SwitchData[3], 0) <<11) |
          (BIT(SwitchData[5], 6) <<12) |
          (BIT(SwitchData[5], 4) <<13) |
          (BIT(SwitchData[5], 2) <<14) |
          (BIT(SwitchData[5], 0) <<15);
  data.Button = (SwitchData[1] << 8) | SwitchData[4];

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

  const uint16_t button_edges = data.Button ^ LastEncoderData.Button;
  if (button_edges) {
      const uint16_t low_edges = button_edges & ~data.Button;
      for (int i = 0; i < Knobs; i++) { // for each encoder
          if (low_edges & (0x8000 >> i)) {
              Gui.Event(construct_event(KNOB_PUSH, i));
          }
      }
  }

  LastEncoderData = data;
}
