/* -*- c++ -*-
 */
#pragma once
#include "logging.h"

template<typename T, int size>
class TCircularBuffer
{
public:
  TCircularBuffer() :
    Read(0), Count(0)
  { }

  bool Put(const T& t)
  {
    if (Count >= size) {
      LOG("No DMA space\n");
      return false;
    }
    Data[(Read + Count) % size] = t;
    Count++;
    LOG("Put DMA job\n");
    return true;
  }

private:
  T Data[size];
  uint8_t Read;
  uint8_t Count;
};
