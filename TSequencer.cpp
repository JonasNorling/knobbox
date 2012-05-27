#include "TSequencer.h"
#include "TMidi.h"
#include "TKnobs.h"
#include "device.h"

void TSequencer::Load()
{
  for (int scene = 0; scene < SceneCount; scene++) {
    Scenes[scene].Magic = TSequencerScene::MAGIC;
    Scenes[scene].Name[0] = 'A';
    Scenes[scene].Name[1] = ' ';
    Scenes[scene].Name[2] = 't';
    Scenes[scene].Name[3] = 'e';
    Scenes[scene].Name[4] = 's';
    Scenes[scene].Name[5] = 't';
    Scenes[scene].Name[6] = ' ';
    Scenes[scene].Name[7] = 's';
    Scenes[scene].Name[8] = 'c';
    Scenes[scene].Name[9] = 'e';
    Scenes[scene].Name[10] = 'n';
    Scenes[scene].Name[11] = 'e';
    
    for (int s = 0; s < SEQLEN; s++) {
      Scenes[scene].Data[s].Note = 0x40;
      Scenes[scene].Data[s].Velocity = 0x40;
    }
  }
}

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
  /// \todo: Calculate an error that we can correct when running
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
  Position.Minor = (Position.Minor + 1) % Resolution;
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
  if (Position.Minor == Resolution - 1) {
    // On each beat, just send a dummy sysex to see the LED blink a bit.
    Midi.EnqueueByte(0xf0);
    Midi.EnqueueByte(0x00);
    Midi.EnqueueByte(0x00);
    Midi.EnqueueByte(0x00);
    Midi.EnqueueByte(0x01);
    Midi.EnqueueByte(0xf7);
  }
}
