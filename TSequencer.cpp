#include "TSequencer.h"
#include "TMidi.h"
#include "TKnobs.h"
#include "device.h"
#include <algorithm>

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

    Scenes[scene].StepLength = 4; // quarter note
    Scenes[scene].Steps = 4;
    
    for (int s = 0; s < SEQLEN; s++) {
      Scenes[scene].Data[s].Flags = Scenes[scene].Data[s].FLAG_ON;
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

  // Running at 96 events per quarter note, we want BPM/60*96
  // interrupts per second.  At 120 BPM that's 192Hz.

  // Counts for a 24MHz CPU clock:
  // 12 BPM --> 1 250 000, 240 BPM --> 62 500
  /// \todo: Calculate an error that we can correct when running
  const uint32_t prescale = 50;
  const uint32_t counts = (rcc_ppre2_frequency * 60 / TicksPerBeat / prescale) / Tempo;

  timer_set_prescaler(TIM2, prescale - 1);
  timer_set_period(TIM2, counts - 1);

  timer_enable_irq(TIM2, TIM_DIER_UIE);
  timer_enable_counter(TIM2);
#endif
}

void TSequencer::Step()
{
  GlobalPosition.Minor++;
  if ((GlobalPosition.Minor % (TicksPerBeat / MidiTicksPerBeat)) == 0) {
    // MIDI clock 24 times per beat
    Midi.SendClockTick();
  }

  const int scene = 0;
  Position[scene].Minor = (Position[scene].Minor + Scenes[scene].StepLength) % TicksPerWholeNote;

  if (Position[scene].Minor == 0) {
    const uint8_t step = (Position[scene].Step + 1) % Scenes[scene].Steps;
    Position[scene].Step = step;
    if (Mode == MODE_SEQ) {
      for (int knob = 0; knob < TKnobs::Knobs; knob++) {
	if (step == knob) {
	  Knobs.LedIntensity[Knobs.COLOR_RED][knob] = 0xff;
	} else {
	  Knobs.LedIntensity[Knobs.COLOR_RED][knob] = 10;
	}
      }
    }

    // Just send some note events for now. We will need to keep track
    // of what has been sent, so that the right off-event is sent even
    // if a knob has been twisted.

    Midi.EnqueueByte(TMidi::MIDI_NOTE_OFF);
    Midi.EnqueueByte(Scenes[scene].Data[(step - 1 + Scenes[scene].Steps) % Scenes[scene].Steps].Note);
    Midi.EnqueueByte(0x40);

    Midi.EnqueueByte(TMidi::MIDI_NOTE_ON);
    Midi.EnqueueByte(Scenes[scene].Data[step].Note);
    Midi.EnqueueByte(Scenes[scene].Data[step].Velocity);
  }
}

void TSequencer::UpdateKnobs() {
  if (Mode == MODE_SEQ) {
    const int scene = 0;
    for (int knob = 0; knob < TKnobs::Knobs; knob++) {
      if (Position[scene].Step == knob) {
	Knobs.LedIntensity[Knobs.COLOR_RED][knob] = 0xff;
      } else {
	Knobs.LedIntensity[Knobs.COLOR_RED][knob] = 10;
      }
      Knobs.LedIntensity[Knobs.COLOR_GREEN][knob] = 10;
    }
  }
}
