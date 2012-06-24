/* -*- c++ -*- */
#pragma once
#include <cstdint>
#include "TMemory.h"
#include "TGui.h"

/**
 * The step sequencer master mind.
 *
 * Status: No real functionality implemented. Design pending.
 */

/**
 * Running at 48 timer ticks per beat (96/s at 120BPM):
 * ticks    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17
 * MIDIclk <0>       <1>       <2>       <3>       <4>       <5>       <6>       <7>       <8>
 * whole    0    1    2    3    4  ......................................................  16  .....
 * quarter  0    4    8   12   16  ......................................................  64  .....
 * 16'th    0   16   32   48   64  .................................  192 wrap
 * 32'th    0   32   64   96  128  160  192 wrap ...................  192 wrap
 */

class TSequencer
{
public:
  static const uint8_t SceneCount = 4;

  TSequencer() :
    Tempo(120),
    Position({ {0, 0}, {0, 0}, {0, 0}, {0, 0} })
  { Load(); }
  void Load();
  void StartTimer();
  void Step();
  uint8_t GetTempo() const { return Tempo; }

  void UpdateKnobs();

  static const char* NoteName(uint8_t n);

  void ChangeNote(int step, int8_t v);
  void ChangeVelocity(int step, int8_t v);
  void ChangeLength(int step, int8_t v);
  void ChangeOffset(int step, int8_t v);

  /// \todo Hide!
  TSequencerScene Scenes[SceneCount];

private:
  static const uint8_t MidiTicksPerBeat = 24;
  static const uint8_t TicksPerBeat = 48; ///< Number of timer events per beat
  static const uint8_t TicksPerWholeNote = TicksPerBeat * 4;

  /// Tempo in BPM (quarter notes per second)
  uint8_t Tempo; ///< Maybe want more precision for tap tempo?

  /// For a quarter note, the sequencer increments the minor step by 4
  /// each tick. So Minor counts up to TicksPerWholeNote (= 192) for
  /// each step.
  struct TPosition {
    uint8_t Step;
    uint8_t Minor;
  };

  TPosition GlobalPosition; ///< \todo Only need minor for MIDI beat
  TPosition Position[SceneCount];
};

extern TSequencer Sequencer;
