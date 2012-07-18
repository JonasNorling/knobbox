/* -*- c++ -*- */
#pragma once
#include <cstdint>
#include "TMemory.h"
#include "TGui.h"
#include "IMidi.h"

static const uint8_t MidiTicksPerBeat = 24;
static const uint8_t TicksPerBeat = 48; ///< Number of timer events per beat

class TPosition {
public:

  /// For a quarter note, the sequencer increments the minor step by 4
  /// each tick. So Minor counts up to TicksPerWholeNote (= 192) for
  /// each step.
  static const uint8_t MinorsPerStep = TicksPerBeat * 4;

  int8_t Step;
  uint8_t Minor;
    
  bool operator==(const TPosition& o) const {
    return o.Step == Step && o.Minor == Minor;
  }

  bool operator>(const TPosition& o) const {
    return (Step > o.Step ||
	    (Step == o.Step && Minor > o.Minor));
  }

  bool operator>=(const TPosition& o) const {
    return (Step > o.Step ||
	    (Step == o.Step && Minor >= o.Minor));
  }

  /// p1 < *this <= p2, with circular wrapping
  inline bool isBetween(const TPosition& p1,
			const TPosition& p2) const;
  inline void AddMinorsAndWrap(int m, uint8_t wrapstep);
};


/**
 * The step sequencer master mind.
 *
 * Status: No real functionality implemented. Design pending.
 *
 * The sequencer contains four (SceneCount) scenes that are run in
 * parallel, but at the same tempo. The scenes can have different step
 * lengths and step counts, so they don't have to run in synchronicity
 * although they use the same base tempo. Each scene is defined in a
 * TSequencerScene instance.
 *
 * The sequencer is advanced by calls to Step() caused by a timer
 * interrupt. Step() is called 48 (TicksPerBeat) times per beat
 * (quarter note). A global position is kept, and it is used to send
 * MIDI clock events 24 (MidiTicksPerBeat) times per beat.
 *
 * The sequencer keeps a position (TPosition) for each scene, which
 * consists of a step counter and a minor (fraction) counter. The pace
 * at which this position is incremented depends on the scene's
 * StepLength.
 *
 * This is how the position's minor tick increments when
 * running at 48 timer ticks per beat (96/s at 120BPM):
 * ticks    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17
 * MIDIclk <0>       <1>       <2>       <3>       <4>       <5>       <6>       <7>       <8>
 * whole    0    1    2    3    4  ......................................................  16  .....
 * quarter  0    4    8   12   16  ......................................................  64  .....
 * 16'th    0   16   32   48   64  .................................  192 wrap
 * 32'th    0   32   64   96  128  160  192 wrap ...................  192 wrap
 *
 */
class TSequencer : public IMemoryCallback
{
public:
  static const uint8_t SceneCount = 4;

  TSequencer(IMidi& midiOutput) :
    MidiOutput(midiOutput),
    Tempo(120),
    Running(false),
    GlobalPosition({0, 0}),
    Position({ {0, 0}, {0, 0}, {0, 0}, {0, 0} }),
    LastPosition({ {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0} }),
    ScheduleLocked(false)
  { }
  void Load();
  void LoadFromMemory(uint8_t scene, uint8_t patchno);
  void StoreInMemory(uint8_t scene, uint8_t patchno);
  void Start(); ///< Start running
  void Stop();
  void Step();
  
  void CalculateSchedule(uint8_t sceneno);
  void UpdateKnobs();

  static const char* NoteName(uint8_t n);

  // Theses methods apply to the active scene
  void ChangeNote(int step, int8_t v);
  void ChangeVelocity(int step, int8_t v);
  void ChangeLength(int step, int8_t v);
  void ChangeOffset(int step, int8_t v);
  void ChangeSteps(int8_t v);
  void ToggleEnable(int step);
  void ChangeStepLength(int8_t v);
  uint8_t GetStepLength() const { return Scenes[0].StepLength; }

  // These methods apply to all scenes
  uint8_t GetTempo() const { return Tempo; }
  void ChangeTempo(int8_t v);
  void ToggleRunning();

  const TPosition& GetGlobalPosition() const
  { return GlobalPosition; }

  const TPosition& GetPosition(uint8_t scene) const
  { return Position[scene]; }

  /// \todo Hide!
  TSequencerScene Scenes[SceneCount];

  // Interface IMemoryCallback
  void MemoryOperationFinished(TMemory::OperationType type,
			       uint8_t block);

private:
  class TEventSchedule {
  public:
    static const int8_t NO_EVENT = -1;

    struct TEntry
    {
      TPosition Position;
      bool IsOn;    ///< Event is a note-on
      uint8_t Step; ///< Step number reference
      int8_t Next;  ///< Link to next event, or NO_EVENT for the last event
      bool LaterThan(const TEntry& o) { return Position > o.Position; }
    };
    static_assert(sizeof(TEntry) == 5, "Strange");

    TEntry Schedule[SEQLEN * 2]; ///< Space for linked entry list
    uint8_t NextFree;  ///< Next available index in Schedule
    int8_t FirstEvent; ///< First event (index) in linked list

    void Clear();
    void Insert(const TEntry& entry);
    void Dump();
  };

  IMidi& MidiOutput;

  /// Tempo in BPM (quarter notes per second)
  uint8_t Tempo; ///< Maybe want more precision for tap tempo?
  bool Running;

  TPosition GlobalPosition; ///< \todo Only need minor for MIDI beat
  TPosition Position[SceneCount];
  TPosition LastPosition[SceneCount]; ///< Last pos that was actually played out

  TEventSchedule Schedule[SceneCount];
  int8_t NextEvent[SceneCount];
  bool ScheduleLocked; ///< Mutex when writing new schedule.

  void ConfigureTimer();
  bool StepIsEnabled(int scene, int step) {
    return step < Scenes[scene].Steps &&
      Scenes[scene].Data[step].Flags & TSequencerScene::TData::FLAG_ON;
  }
  void DoNextEvent(int sceneno);
  void PlayEvent(int sceneno, const TEventSchedule::TEntry& event);
};

extern TSequencer Sequencer;
