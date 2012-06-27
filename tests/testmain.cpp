#include <cstdio>

/* This is the price of poor decoupling. But I'm not going to pay for
   virtual functions and crap. */

#include "TDisplay.h"
#include "TSpiDmaJob.h"
#include "TGui.h"
#include "TKnobs.h"
#include "TSequencer.h"
#include "TMidi.h"
#include "TMemory.h"
#include "TControllers.h"

int Mode;
TDisplay Display;
TSpiDmaQueue SpiDmaQueue;
TGui Gui;
TKnobs Knobs;
TSequencer Sequencer;
TMidi Midi;
TMemory Memory;
TControllers Controllers;

void dma1_channel2_isr()
{
}

/*
 * ------------------------------------------------------------
 */

#define test_assert(x) if (!(x)) { fprintf(stderr, "%s:%d: error: Test failed: %s\n", __FILE__, __LINE__, #x); return 1; }

#include "TestCircularBuffer.cpp"
#include "TestMidi.cpp"
#include "TestSequencer.cpp"

int main(int, char**)
{
  fprintf(stderr, "Running tests...\n");

  int failures = 0;
  failures += TestCircularBuffer();
  failures += TestMidi();
  failures += TestSequencer();

  if (failures == 0) {
    fprintf(stderr, "Tests OK.\n");
  }
  else {
    fprintf(stderr, "error: %d tests failed.\n", failures);
  }

  return failures;
}
