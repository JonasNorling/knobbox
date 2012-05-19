#include "TSequencer.h"
#include "TMidi.h"
#include "TKnobs.h"
#include "device.h"

void TSequencer::StartTimer()
{
#ifndef HOST
  TIM_CNT(TIM2) = 1;

  // Counter frequency = cpu frequency / (prescaler + 1)
  // Interrupt frequency = counter frequency / (period + 1)
  // ...so the interrupt frequency is system clock divided by
  // (prescaler + 1) * (period + 1)

  // Running at 48 events per quarter note, we want BPM/60*48
  // interrupts per second.  At 120 BPM that's 96Hz.

  // Counts for a 24MHz CPU clock:
  // 12 BPM --> 2 500 000, 240 BPM --> 125 000
  // FIXME: Calculate an error that we can correct when running
  const uint32_t prescale = 50;
  const uint32_t counts = (rcc_ppre2_frequency * 60 / Resolution / prescale) / Tempo;

  timer_set_prescaler(TIM2, prescale - 1);
  timer_set_period(TIM2, counts - 1);

  timer_enable_irq(TIM2, TIM_DIER_UIE);
  timer_enable_counter(TIM2);
#endif
}

void TSequencer::Step()
{
  Position.Minor = (Position.Minor + 1 ) % Resolution;
#ifndef HOST
  if (Position.Minor == 0) {
    Pin_led_b.Set();
    for (int i=0; i < Knobs.Knobs; i++) {
      Knobs.LedIntensity[1][i] = 10;
    }
  }
  else if (Position.Minor == 2) {
    Pin_led_b.Clear();
    for (int i=0; i < Knobs.Knobs; i++) {
      Knobs.LedIntensity[1][i] = 0;
    }
  }
#endif
  if ((Position.Minor % (Resolution / 24)) == 0) {
    Midi.SendClockTick();
  }
}
