/* -*- c++ -*-
 */
#pragma once
#include <cstdint>
#include "stm32.h"
#include "TCircularBuffer.h"

class TBuffer;

class IDmaCallback
{
public:
  virtual void DmaFinished(void* context) = 0;
};

class TSpiDmaJob
{
public:
  enum TChip {
    CS_FLASH = 0,
    CS_LCD = 1
  };
  enum TLcdCd {
    LCD_CONTROL = 0,
    LCD_DATA = 1
  };

  TSpiDmaJob() {}

  TSpiDmaJob(const TBuffer& data,
	     TChip chip,
	     TLcdCd lcdCd,
	     IDmaCallback* callback = 0,
	     void* context = 0) :
    Data(&data),
    Callback(callback),
    Context(context),
    Bits((chip << 1) | lcdCd)
  { }

  void Finished() const
  {
    if (Callback) {
      Callback->DmaFinished(Context);
    }
  }    

private:
  const TBuffer* Data;
  IDmaCallback* Callback;
  void* Context;
  uint8_t Bits;
};

class TSpiDmaQueue
{
public:
  TSpiDmaQueue()
  {
#ifndef HOST
    dma_channel_reset(DMA1, DMA_CHANNEL7);
#endif
  }
  
  bool Enqueue(const TSpiDmaJob& job)
  {
    if (Jobs.Add(job)) {
      // FIXME: If no job is under way, start the DMA engine.  Think
      // about race conditions. What if the interrupt happens when we
      // have decided not to start a job, but before it's put on the
      // queue. The solution is to enqueue it first, always?

      Jobs.First();

#ifdef HOST
      dma1_channel7_isr();
#else
#endif
      return true;
    } else {
      return false;
    }
  }

  /** Called when interrupt is received. */
  void Finished()
  {
    Jobs.First().Finished();
    Jobs.Remove();
  }

private:
  // Max jobs needed: 2 for each TDisplay buffer + 1 for each flash operation
  static const int MaxJobs = 5;
  TCircularBuffer<TSpiDmaJob, MaxJobs> Jobs;
};

extern TSpiDmaQueue SpiDmaQueue;
