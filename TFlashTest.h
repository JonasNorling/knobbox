/* -*- c++ -*- */
#pragma once

#include "TFlash.h"

class TFlashTest : public IFlashCallback
{
public:
  TFlashTest() : State(0) {}

  void RunRead();
  void RunWrite();

  void FlashJobFinished(TFlashJob& job);

  int State;
};
