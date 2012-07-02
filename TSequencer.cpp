#include "TSequencer.h"
#include "TMidi.h"
#include "TKnobs.h"
#include "device.h"
#include "utils.h"
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
      Scenes[scene].Data[s].Len = 24;
      Scenes[scene].Data[s].Offset = 0;
    }
  }
}

void TSequencer::Start()
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

  Running = true;
}

void TSequencer::Stop()
{
#ifndef HOST
  timer_disable_counter(TIM2);
#endif

  Running = false;
}

/**
 * Called on timer tick 48 times per beat. Update global and
 * scene-local positions, cause sequencer to think about what to do
 * next.
 */
void TSequencer::Step()
{
  GlobalPosition.Minor++;
  if ((GlobalPosition.Minor % (TicksPerBeat / MidiTicksPerBeat)) == 0) {
    // MIDI clock 24 times per beat
    Midi.SendClockTick();
  }

  for (int scene = 0; scene < SceneCount; scene++) {
    Position[scene].Minor += Scenes[scene].StepLength;
    if (Position[scene].Minor >= TPosition::MinorsPerStep) {
      Position[scene].Minor -= TPosition::MinorsPerStep;
      Position[scene].Step = (Position[scene].Step + 1) % Scenes[scene].Steps;
    }
    DoNextEvent(scene);
  }

  UpdateKnobs();
}

/**
 * \todo This is obviously extremely naive.
 */
void TSequencer::DoNextEvent(int sceneno)
{
  // Just play one scene during development, it's complicated enough.
  if (sceneno != 0) return;

  /*
   * For each step, find out if it's time to send a note on or off
   * event. It's not clear how much CPU time this takes, but it's
   * probably excessive (and we're in interrupt context here).
   *
   * \todo It would be better to calculate a sorted list of events
   * when a change happens (not in interrupt context), and just pop
   * off the next event when the time advances.
   */
  const TSequencerScene& scene(Scenes[sceneno]);
  for (int step=0; step < scene.Steps; step++) {
    const TSequencerScene::TData& data = scene.Data[step];

    TPosition due({static_cast<uint8_t>(step), 0});
    due.AddMinorsAndWrap(data.Offset * 4, scene.Steps);

    TPosition due_end(due);
    due_end.AddMinorsAndWrap(data.Len * 4, scene.Steps);

    if (StepIsEnabled(sceneno, step)) {
      if (Position[sceneno] == due) {
	Midi.EnqueueByte(TMidi::MIDI_NOTE_ON);
	Midi.EnqueueByte(data.Note);
	Midi.EnqueueByte(data.Velocity);
	if (Mode == MODE_SEQ) {
	  Knobs.LedIntensity[Knobs.COLOR_GREEN][step] = 0x80;
	}
      }
      if (Position[sceneno] == due_end) {
	Midi.EnqueueByte(TMidi::MIDI_NOTE_OFF);
	Midi.EnqueueByte(data.Note);
	Midi.EnqueueByte(0x40);
	if (Mode == MODE_SEQ) {
	  Knobs.LedIntensity[Knobs.COLOR_GREEN][step] = 0;
	}
      }
    }
  }
}

/**
 * Update knob colors. Highlight the knob representing the current beat.
 */
void TSequencer::UpdateKnobs()
{
  if (Mode == MODE_SEQ) {
    const int scene = 0;
    for (int knob = 0; knob < TKnobs::Knobs; knob++) {
      if (Position[scene].Step == knob) {
	Knobs.LedIntensity[Knobs.COLOR_RED][knob] = 0xff;
      } else {
	if (StepIsEnabled(0, knob)) {
	  Knobs.LedIntensity[Knobs.COLOR_RED][knob] = 5;
	}
	else {
	  Knobs.LedIntensity[Knobs.COLOR_RED][knob] = 0;
	}
      }
    }
  }
}

const char* TSequencer::NoteName(uint8_t n)
{
  static char name[4];

  if (n < TMidi::MIDI_NOTE_MIN || n > TMidi::MIDI_NOTE_MAX) {
    name[0] = 'X';
    name[1] = '\0';
    return name;
  }

  const int notes_per_octave = 12;
  static const char* names[notes_per_octave] = {
    "C", "C#", "D", "Eb", "E", "F",
    "F#", "G", "G#", "A", "Bb", "B" };

  int octave = (n - TMidi::MIDI_NOTE_C0) / notes_per_octave;
  const char* notename = names[(n - TMidi::MIDI_NOTE_C0) % notes_per_octave];

  size_t pos = cheap_strcpy(name, notename);
  render_uint(&name[pos], octave, 1);
  name[pos+1] = '\0';

  return name;
}

void TSequencer::ChangeNote(int step, int8_t v)
{
  Scenes[0].Data[step].Note = clamp(Scenes[0].Data[step].Note + v,
				    TMidi::MIDI_NOTE_MIN, TMidi::MIDI_NOTE_MAX);
}

void TSequencer::ChangeVelocity(int step, int8_t v)
{
  Scenes[0].Data[step].Velocity = clamp(Scenes[0].Data[step].Velocity + v,
					0, 127);
}

void TSequencer::ChangeLength(int step, int8_t v)
{
  Scenes[0].Data[step].Len = clamp(Scenes[0].Data[step].Len + v,
				   0, 240);
}

void TSequencer::ChangeOffset(int step, int8_t v)
{
  Scenes[0].Data[step].Offset = clamp(Scenes[0].Data[step].Offset + v,
				      -120, 120);
}

void TSequencer::ChangeTempo(int8_t v)
{
  Tempo = clamp(Tempo + v, 20, 255);
  if (Running) Start(); // Reconfigure timer
}

void TSequencer::ToggleEnable(int step)
{
  Scenes[0].Data[step].Flags = Scenes[0].Data[step].Flags ^ TSequencerScene::TData::FLAG_ON;
  /// \todo Kill note and turn off LED if disabled while note is playing.
  UpdateKnobs();
}

void TSequencer::ToggleRunning()
{
  if (Running) {
    Stop();
  }
  else {
    Start();
  }
}

void TSequencer::ChangeStepLength(int8_t v)
{
  Scenes[0].StepLength = clamp(Scenes[0].StepLength + v, 0, 96);
}
