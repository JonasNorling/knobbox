#include "TSequencer.h"
#include "TMidi.h"
#include "TKnobs.h"
#include "device.h"
#include "utils.h"
#include "logging.h"
#include <algorithm>
#include <cstring>

void TPosition::AddMinorsAndWrap(int m, uint8_t wrapstep)
{
    m += Minor;

    if (m < 0) {
        const int wrapminors = wrapstep * MinorsPerStep;
        m = wrapminors + (m % wrapminors);
    }
    Minor = m % MinorsPerStep;
    Step = (Step + m / MinorsPerStep) % wrapstep;
}

bool TPosition::isBetween(const TPosition& p1, const TPosition& p2) const
{
    if (p2 > p1) {
        // no wrapping
        return *this > p1 && p2 >= *this;
    }
    else {
        return *this > p1 || p2 >= *this;
    }
}

/*
 * *******************************************************************
 */

void TSequencer::Load()
{
    for (int scene = 0; scene < SceneCount; scene++) {
        Scenes[scene].Magic = TSequencerScene::MAGIC;
        Scenes[scene].Name[0] = 'U';
        Scenes[scene].Name[1] = 'n';
        Scenes[scene].Name[2] = 'n';
        Scenes[scene].Name[3] = 'a';
        Scenes[scene].Name[4] = 'm';
        Scenes[scene].Name[5] = 'e';
        Scenes[scene].Name[6] = 'd';
        Scenes[scene].Name[7] = ' ';
        Scenes[scene].Name[8] = '0' + scene;
        Scenes[scene].Name[9] = '\0';
        Scenes[scene].Name[10] = '\0';
        Scenes[scene].Name[11] = '\0';

        Scenes[scene].Flags = scene == 0 ? TSequencerScene::FLAG_ENABLED : 0;
        Scenes[scene].Channel = 0;
        Scenes[scene].StepLength = 4; // quarter note
        Scenes[scene].Steps = 4;

        for (int s = 0; s < SEQLEN; s++) {
            Scenes[scene].Data[s].Flags = Scenes[scene].Data[s].FLAG_ON;
            Scenes[scene].Data[s].Note = 0x40;
            Scenes[scene].Data[s].Velocity = 0x40;
            Scenes[scene].Data[s].Len = 24;
            Scenes[scene].Data[s].Offset = 0;
        }
        CalculateSchedule(scene);
    }
}

void TSequencer::LoadFromMemory(uint8_t /*scene*/, uint8_t patchno)
{
    // Suck in the block from memory. When that's done, a
    // MemoryOperationFinished() will be triggered.
    //
    // \todo How is the GUI indicating that a load is in progress? What
    // happens on failure? Magic number must be verified.
    Memory.FetchBlock(TMemory::BLOCK_FIRST_SEQ_SCENE + patchno, this);
}

void TSequencer::StoreInMemory(uint8_t scene, uint8_t patchno)
{
    ::memset(Memory.GetCachedBlock(), 0, TMemory::BlockSize);
    ::memcpy(Memory.GetCachedBlock(), &Scenes[scene], sizeof(TSequencerScene));

    TSequencerScene& sceneCopy(*reinterpret_cast<TSequencerScene*>(Memory.GetCachedBlock()));
    for (int step = 0; step < SEQLEN; step++) {
        TSequencerScene::TData& data = sceneCopy.Data[step];
        data.Flags &= TSequencerScene::TData::FLAG_SAVE_MASK;
    }

    Memory.WriteBlock(TMemory::BLOCK_FIRST_SEQ_SCENE + patchno, this);
}

void TSequencer::MemoryOperationFinished(TMemory::OperationType type,
        uint8_t /*block*/)
{
    if (type == TMemory::OP_READ) {
        const TSequencerScene* s =
                reinterpret_cast<TSequencerScene*>(Memory.GetCachedBlock());
        if (s->Magic == TSequencerScene::MAGIC) {
            ::memcpy(&Scenes[CurrentScene], s, sizeof(TSequencerScene));
            CalculateSchedule(CurrentScene);
        }
        Gui.UpdateAll();
    }
    else if (type == TMemory::OP_WRITE) {
    }
}

/**
 * Start the sequencer timer. The next event will be triggered soon.
 */
void TSequencer::Start()
{
    Running = true;
    ConfigureTimer();
}

void TSequencer::ConfigureTimer()
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
 *
 * WE ARE IN TIM2 INTERRUPT CONTEXT HERE
 */
void TSequencer::Step()
{
    GlobalPosition.Minor++;
    if ((GlobalPosition.Minor % (TicksPerBeat / MidiTicksPerBeat)) == 0) {
        // MIDI clock 24 times per beat
        MidiOutput.SendClockTick();
    }

    for (int scene = 0; scene < SceneCount; scene++) {
        TPosition& position = Position[scene];
        position.Minor += Scenes[scene].StepLength;
        if (position.Minor >= TPosition::MinorsPerStep) {
            position.Minor -= TPosition::MinorsPerStep;
            position.Step = (position.Step + 1) % Scenes[scene].Steps;
        }
        DoNextEvent(scene);
    }

    UpdateKnobs();
}

/**
 * Go through all steps and schedule their events (note on/off, ...)
 * in an ordered list.
 *
 * Events are pointers to a step. When playing back the event list,
 * the code has to refer to the scene data to read note values and
 * velocities, etc.
 */
void TSequencer::CalculateSchedule(uint8_t sceneno)
{
    ScheduleLocked = true;
    TEventSchedule& schedule = Schedule[sceneno];
    schedule.Clear();

    const TSequencerScene& scene(Scenes[sceneno]);

    if (scene.Flags & TSequencerScene::FLAG_ENABLED) {
        for (uint8_t step=0; step < scene.Steps; step++) {
            const TSequencerScene::TData& data = scene.Data[step];
            if (StepIsEnabled(sceneno, step)) {
                TPosition starttime({static_cast<int8_t>(step), 0});
                starttime.AddMinorsAndWrap(data.Offset * 4, scene.Steps);
                TPosition endtime(starttime);
                endtime.AddMinorsAndWrap(data.Len * 4, scene.Steps);

                TEventSchedule::TEntry onEvent = { starttime, true, step, -1 };
                TEventSchedule::TEntry offEvent = { endtime, false, step, -1 };

                schedule.Insert(onEvent);
                schedule.Insert(offEvent);
            }
        }
    }

    // Find the next event to play
    int8_t event = schedule.FirstEvent;
    NextEvent[sceneno] = schedule.FirstEvent;
    while (event != TEventSchedule::NO_EVENT) {
        if (schedule.Schedule[event].Position >= Position[sceneno]) {
            NextEvent[sceneno] = event;
            break;
        }
        event = schedule.Schedule[event].Next;
    }

    ScheduleLocked = false;
    //schedule.Dump();
}

/*
 * Play out the next event if the time has come to do so.
 *
 * WE MIGHT BE IN TIM2 INTERRUPT CONTEXT HERE
 */
void TSequencer::DoNextEvent(int sceneno)
{
    // If the schedule is being updated we'll have to wait for the next tick.
    if (ScheduleLocked) return;

    int8_t& nextEvent = NextEvent[sceneno];
    while (nextEvent != TEventSchedule::NO_EVENT) {
        const TEventSchedule& schedule = Schedule[sceneno];
        const TEventSchedule::TEntry& next = schedule.Schedule[nextEvent];

        if (next.Position.isBetween(LastPosition[sceneno],
                Position[sceneno])) {
            /* LOG("Playing %s %d:%d when pos=%d:%d\n",
					next.IsOn ? " on" : "off",
					next.Position.Step, next.Position.Minor,
					position.Step, position.Minor); */

            PlayEvent(sceneno, next);

            nextEvent = next.Next;
            if (nextEvent == TEventSchedule::NO_EVENT) {
                nextEvent = schedule.FirstEvent;
                // \todo We really can't give up here -- the position could
                // have wrapped and there could be events in the beginning of
                // the list to play. We need to keep track of the start point
                // of list traversion, so we don't play the same events in an
                // ininite loop here. For now, just bail out.
                break;
            }
        }
        else {
            // Wait for time to advance
            break;
        }
    }
    LastPosition[sceneno] = Position[sceneno];
}

void TSequencer::PlayEvent(int sceneno, const TEventSchedule::TEntry& event)
{
    TSequencerScene& scene(Scenes[sceneno]);
    TSequencerScene::TData& data = scene.Data[event.Step];

    if (data.Flags & TSequencerScene::TData::FLAG_SOUNDING) {
        NoteOff(scene, event.Step);
    }
    if (event.IsOn) {
        NoteOn(scene, event.Step);
    }
}

/**
 * Update knob colors. Highlight the knob representing the current beat.
 *
 * WE MIGHT BE IN TIM2 INTERRUPT CONTEXT HERE
 */
void TSequencer::UpdateKnobs()
{
    if (Mode == MODE_SEQ) {
        const int scene = CurrentScene;

        for (int knob = 0; knob < TKnobs::Knobs; knob++) {
            if (Position[scene].Step == knob) {
                Knobs.LedIntensity[Knobs.COLOR_RED][knob] = 0xff;
            }
            else if (StepIsEnabled(scene, knob)) {
                Knobs.LedIntensity[Knobs.COLOR_RED][knob] = 5;
            }
            else {
                Knobs.LedIntensity[Knobs.COLOR_RED][knob] = 0;
            }

            if (Scenes[scene].Data[knob].Flags & TSequencerScene::TData::FLAG_SOUNDING) {
                Knobs.LedIntensity[Knobs.COLOR_GREEN][knob] = 0x80;
            }
            else {
                Knobs.LedIntensity[Knobs.COLOR_GREEN][knob] = 0x0;
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
    TSequencerScene& scene(Scenes[CurrentScene]);
    TSequencerScene::TData& data = scene.Data[step];

    // If note is sounding, kill it
    bool sounding = data.Flags & TSequencerScene::TData::FLAG_SOUNDING;
    if (sounding) {
        NoteOff(scene, step);
    }

    data.Note = clamp(data.Note + v, TMidi::MIDI_NOTE_MIN, TMidi::MIDI_NOTE_MAX);

    // If note is sounding, play the new one
    if (sounding) {
        NoteOn(scene, step);
    }
}

void TSequencer::ChangeVelocity(int step, int8_t v)
{
    Scenes[CurrentScene].Data[step].Velocity = clamp(Scenes[CurrentScene].Data[step].Velocity + v, 0, 127);
}

void TSequencer::ChangeLength(int step, int8_t v)
{
    // FIXME: Kill sounding note if applicable
    Scenes[CurrentScene].Data[step].Len = clamp(Scenes[CurrentScene].Data[step].Len + v, 0, 240);
    CalculateSchedule(CurrentScene);
}

void TSequencer::ChangeOffset(int step, int8_t v)
{
    // FIXME: Kill sounding note if applicable
    Scenes[CurrentScene].Data[step].Offset = clamp(Scenes[CurrentScene].Data[step].Offset + v, -120, 120);
    CalculateSchedule(CurrentScene);
}

void TSequencer::ChangeTempo(int8_t v)
{
    Tempo = clamp(Tempo + v, 20, 255);
    if (Running) ConfigureTimer();
}

void TSequencer::ChangeSteps(int8_t v)
{
    // FIXME: Kill sounding note if applicable
    Scenes[CurrentScene].Steps = clamp(Scenes[CurrentScene].Steps + v, 1, 16);
    CalculateSchedule(CurrentScene);
}

void TSequencer::ToggleEnable(int step)
{
    TSequencerScene& scene(Scenes[CurrentScene]);
    TSequencerScene::TData& data = scene.Data[step];

    // If note is sounding, kill it
    if (data.Flags & TSequencerScene::TData::FLAG_SOUNDING) {
        NoteOff(scene, step);
    }

    data.Flags = data.Flags ^ TSequencerScene::TData::FLAG_ON;
    CalculateSchedule(CurrentScene);
    UpdateKnobs();
}

void TSequencer::ToggleSceneEnable(int sceneno)
{
    TSequencerScene& scene(Scenes[sceneno]);
    scene.Flags ^= TSequencerScene::FLAG_ENABLED;

    // FIXME: If note is sounding, kill it

    CalculateSchedule(sceneno);
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
    Scenes[CurrentScene].StepLength = clamp(Scenes[CurrentScene].StepLength + v, 0, 96);
}

void TSequencer::NoteOff(TSequencerScene& scene, uint8_t step)
{
    TSequencerScene::TData& data = scene.Data[step];
    MidiOutput.SendEvent(TMidi::MIDI_NOTE_OFF | scene.Channel, data.Note, 0x40);
    data.Flags &= ~TSequencerScene::TData::FLAG_SOUNDING;
}

void TSequencer::NoteOn(TSequencerScene& scene, uint8_t step)
{
    TSequencerScene::TData& data = scene.Data[step];
    MidiOutput.SendEvent(TMidi::MIDI_NOTE_ON | scene.Channel, data.Note, data.Velocity);
    data.Flags |= TSequencerScene::TData::FLAG_SOUNDING;
}

/*
 * *******************************************************************
 */

void TSequencer::TEventSchedule::Clear()
{
    FirstEvent = NO_EVENT;
    NextFree = 0;
}

void TSequencer::TEventSchedule::Insert(const TEntry& e)
{
    TEntry& entry = Schedule[NextFree];
    entry = e;
    //LOG("Insert at %d:%d\n", entry.Position.Step, entry.Position.Minor);

    int8_t lastpos = NO_EVENT;
    int8_t pos = FirstEvent;
    while (pos != NO_EVENT) {
        if (entry.LaterThan(Schedule[pos])) {
            //LOG("  later than\n");
            lastpos = pos;
            pos = Schedule[pos].Next;
        }
        else {
            //LOG("  earlier than\n");
            break;
        }
    }

    entry.Next = pos;
    if (lastpos == NO_EVENT) {
        FirstEvent = NextFree;
    }
    else {
        Schedule[lastpos].Next = NextFree;
    }

    NextFree++;
}

void TSequencer::TEventSchedule::Dump()
{
#ifdef HOST
    LOG("Schedule dump (NextFree=%d, FirstEvent=%d)\n",
            NextFree, FirstEvent);

    int8_t pos = FirstEvent;
    while (pos != NO_EVENT) {
        const TEntry& e = Schedule[pos];
        LOG("   [%d]: %2d:%03d step %d %s --> [%d]\n",
                pos, e.Position.Step, e.Position.Minor,
                e.Step, e.IsOn ? "on " : "off",	e.Next);
        pos = e.Next;
    }
    LOG("   end.\n");
#endif
}
