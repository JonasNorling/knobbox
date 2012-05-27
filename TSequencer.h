/* -*- c++ -*- */
#pragma once
#include <cstdint>
#include "TMemory.h"

/**
 * The step sequencer master mind.
 *
 * Status: No real functionality implemented. Design pending.
 */
class TSequencer
{
public:
  TSequencer() : Tempo(120), Position({0, 0}) { Load(); }
  void Load();
  void StartTimer();
  void Step();
  uint8_t GetTempo() const { return Tempo; }

private:
  static const uint8_t SceneCount = 4;
  static const uint8_t Resolution = 48; // Events per quarter note
  // Tempo in BPM (quarter notes per second)

  uint8_t Tempo; // Maybe want more precision for tap tempo?
  struct {
    uint8_t Beats;
    uint8_t Minor;
  } Position;

  TSequencerScene Scenes[SceneCount];
};

extern TSequencer Sequencer;
