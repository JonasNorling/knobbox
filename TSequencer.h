/* -*- c++ -*- */
#pragma once
#include <cstdint>
#include "TMemory.h"
#include "TGui.h"

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

class TSequencer
{
public:
  static const uint8_t SceneCount = 4;

  TSequencer() :
    Tempo(120),
    Running(false),
    Position({ {0, 0}, {0, 0}, {0, 0}, {0, 0} })
  { Load(); }
  void Load();
  void Start();
  void Stop();
  void Step();
  uint8_t GetTempo() const { return Tempo; }

  void UpdateKnobs();

  static const char* NoteName(uint8_t n);

  void ChangeNote(int step, int8_t v);
  void ChangeVelocity(int step, int8_t v);
  void ChangeLength(int step, int8_t v);
  void ChangeOffset(int step, int8_t v);
  void ToggleEnable(int step);
  void ToggleRunning();

  /// \todo Hide!
  TSequencerScene Scenes[SceneCount];

private:
  static const uint8_t MidiTicksPerBeat = 24;
  static const uint8_t TicksPerBeat = 48; ///< Number of timer events per beat

  class TPosition {
  public:
    uint8_t Step;
    uint8_t Minor;
    
    bool operator==(const TPosition& o) const {
      return o.Step == Step && o.Minor == Minor;
    }
  };

  /// Tempo in BPM (quarter notes per second)
  uint8_t Tempo; ///< Maybe want more precision for tap tempo?
  bool Running;

  /// For a quarter note, the sequencer increments the minor step by 4
  /// each tick. So Minor counts up to TicksPerWholeNote (= 192) for
  /// each step.
  static const uint8_t MinorsPerStep = TicksPerBeat * 4;

  TPosition GlobalPosition; ///< \todo Only need minor for MIDI beat
  TPosition Position[SceneCount];

  bool StepIsEnabled(int scene, int step) {
    return Scenes[scene].Data[step].Flags & TSequencerScene::TData::FLAG_ON;
  }
  void DoNextEvent(int sceneno);
};

extern TSequencer Sequencer;
