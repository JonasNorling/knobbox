/* -*- c++ -*- */
#pragma once

#include "utils.h"
#include "TMemory.h"
#include "TKnobs.h"
#include "TGui.h"
#include "TMidi.h"

/**
 * Keep controller (parameters/knobs) state. Receive change events,
 * make the appropriate calls.
 *
 * Status: This is a sketch. Functionality is missing.
 */
class TControllers
{
public:
  const static uint8_t MIN = 0;
  const static uint8_t MAX = 0x7f;

  TControllers() { Load(); }

  /// Load the controller scene from flash.
  void Load();
  const TParamScene& GetScene() const { return Scene; }
  uint8_t GetValue(int knob) const { return Values[knob]; }
  int GetActiveKnob() const { return 2; }

  void IncreaseValue(int knob, uint8_t v) {
    Values[knob] += v;
    if (Values[knob] > MAX) Values[knob] = MAX;
    Knobs.LedIntensity[Knobs.COLOR_RED][knob] = 255-2*Values[knob];
    Knobs.LedIntensity[Knobs.COLOR_GREEN][knob] = 2*Values[knob];
    SendMidi(knob);
    Gui.UpdateLine(3);
    Gui.UpdateLine(4);
  }
  void DecreaseValue(int knob, uint8_t v) {
    Values[knob] -= v;
    if (Values[knob] > MAX) Values[knob] = MIN;
    Knobs.LedIntensity[Knobs.COLOR_RED][knob] = 255-2*Values[knob];
    Knobs.LedIntensity[Knobs.COLOR_GREEN][knob] = 2*Values[knob];
    SendMidi(knob);
    Gui.UpdateLine(3);
    Gui.UpdateLine(4);
  }

  uint8_t Values[KNOBS];

private:
  void SendMidi(int knob)
  {
    Midi.EnqueueByte(TMidi::MIDI_CC | Scene.Knobs[knob].Channel);
    Midi.EnqueueByte(Scene.Knobs[knob].Param);
    Midi.EnqueueByte(Values[knob]);
  }

  TParamScene Scene;
};

extern TControllers Controllers;
