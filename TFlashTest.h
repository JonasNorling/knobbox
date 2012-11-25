/* -*- c++ -*- */
#pragma once

#include "TMemory.h"

class TFlashTest : public IMemoryCallback
{
public:
  TFlashTest() : State(0) {}

  void RunRead();
  void RunWrite();

  // Interface IMemoryCallback
  void MemoryOperationFinished(TMemory::OperationType type, uint8_t block);

private:
  int State;
};
