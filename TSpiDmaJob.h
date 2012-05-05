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

  TSpiDmaJob(const TBuffer& buffer,
	     TChip chip,
	     TLcdCd lcdCd,
	     IDmaCallback* callback = 0,
	     void* context = 0) :
    Buffer(&buffer),
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

  const TBuffer& GetBuffer() const { return *Buffer; }
  TChip GetChip() const { return static_cast<TChip>(Bits >> 1); }
  TLcdCd GetLcdData() const { return static_cast<TLcdCd>(Bits & 1); }

private:
  const TBuffer* Buffer;
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
  
  bool Enqueue(const TSpiDmaJob& job);
  void TryStartJob();

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
