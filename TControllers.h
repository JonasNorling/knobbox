/* -*- c++ -*- */
#pragma once

#include "utils.h"
#include "TMemory.h"
#include "TKnobs.h"
#include "TGui.h"

class TControllers
{
public:
  const static uint8_t MIN = 0;
  const static uint8_t MAX = 0x7f;

  TControllers() { Load(); }

  void Load();
  const TParamScene& GetScene() const { return Scene; }
  uint8_t GetValue(int knob) const { return Values[knob]; }
  int GetActiveKnob() const { return 2; }
  void IncreaseValue(int knob, uint8_t v) {
    Values[knob] += v;
    if (Values[knob] > MAX) Values[knob] = MAX;
    Gui.UpdateLine(3);
    Gui.UpdateLine(4);
  }
  void DecreaseValue(int knob, uint8_t v) {
    Values[knob] -= v;
    if (Values[knob] > MAX) Values[knob] = MIN;
    Gui.UpdateLine(3);
    Gui.UpdateLine(4);
  }

  uint8_t Values[KNOBS];

private:
  TParamScene Scene;
};

extern TControllers Controllers;
