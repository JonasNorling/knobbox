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
#include "TScheduler.h"

TScheduler::TTaskControlBlock TScheduler::Tcbs[SCHEDULER_NUM_TASKS];
uint8_t TScheduler::CurrentTask;
const TScheduler::TTask TScheduler::Tasks[SCHEDULER_NUM_TASKS] = {
        { "main", 0, 0, 0 },
        { "gui", 0, 0, 0 },
};

int Mode;
TDisplay Display;
TSpiDmaQueue SpiDmaQueue;
TGui Gui;
TKnobs Knobs;
TMidi Midi;
TSequencer Sequencer(Midi);
TMemory Memory;
TControllers Controllers(Midi);
TFlash Flash;

volatile uint32_t SystemTime = 0; // in ms, wraps at 49.7 days

void dma1_channel2_isr()
{
}

void dma1_channel4_isr(void)
{
}

void sys_tick_handler(void)
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
  failures += TestMidiParser();
  failures += TestSequencer();

  if (failures == 0) {
    fprintf(stderr, "Tests OK.\n");
  }
  else {
    fprintf(stderr, "error: %d tests failed.\n", failures);
  }

  return failures;
}
