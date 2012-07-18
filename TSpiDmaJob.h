/* -*- c++ -*-
 */
#pragma once
#include <cstdint>
#include "device.h"
#include "TCircularBuffer.h"
#include "TBuffer.h"

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
    Buffer(buffer),
    Callback(callback),
    Context(context),
    Bits((chip << 1) | lcdCd)
  { }

  void Finished() const;
  const TBuffer& GetBuffer() const { return Buffer; }
  TChip GetChip() const { return static_cast<TChip>(Bits >> 1); }
  TLcdCd GetLcdData() const { return static_cast<TLcdCd>(Bits & 1); }

private:
  TBuffer Buffer;
  IDmaCallback* Callback;
  void* Context;
  uint8_t Bits;
};

class TSpiDmaQueue
{
public:
  TSpiDmaQueue();
  
  bool Enqueue(const TSpiDmaJob& job);
  void TryStartJob();

  /** Called when interrupt is received. */
  void Finished()
  {
    TSpiDmaJob job = Jobs.First();
    Jobs.Remove();

    if (Jobs.Empty()) {
      // Deassert CS lines if no more transactions are planned. We
      // need to do this before the callback, because that can enqueue
      // more transactions. CS needs to be deasserted between each
      // memory access.
      Pin_flash_cs.Set();
      Pin_lcd_cs.Set();
    }

    job.Finished();
  }

private:
  // Max jobs needed: 2 for each TDisplay buffer + 2 for each flash
  // operation
  static const int MaxJobs = 6;
  TCircularBuffer<TSpiDmaJob, MaxJobs> Jobs;
};

extern TSpiDmaQueue SpiDmaQueue;
