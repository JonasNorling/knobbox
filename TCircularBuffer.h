/* -*- c++ -*-
 */
#pragma once
#include "logging.h"

#ifdef HOST
#include <cassert>
#else
#define assert(x)
#endif

template<typename T, int size>
class TCircularBuffer
{
public:
  TCircularBuffer() :
    Read(0), Count(0)
  { }

  bool Add(const T& t)
  {
    LOG("TCircularBuffer::Add\n");
    if (Count >= size) {
      LOG("TCircularBuffer::Add: no space left\n");
      return false;
    }
    Data[(Read + Count) % size] = t;
    Count++;
    return true;
  }

  void Remove()
  {
    LOG("TCircularBuffer::Remove\n");
    assert(Count > 0);
    Read = (Read + 1) % size;
    Count--;
  }

  const T& First()
  {
    assert(Count > 0);
    return Data[Read];
  }

private:
  T Data[size];
  uint8_t Read;
  uint8_t Count;
};
