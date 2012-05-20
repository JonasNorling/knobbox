/* -*- c++ -*- */
#pragma once
#include "logging.h"
#include <cstdint>

#ifdef HOST
#include <cassert>
#else
#define assert(x)
#endif

/**
 * Circular buffer class. Holds size objects of type T.
 *
 * Status: Unit tested. Works.
 */
template<typename T, int size>
class TCircularBuffer
{
public:
  TCircularBuffer() :
    Read(0), Count(0)
  { }

  /// Add object to end of buffer.
  bool Add(const T& t)
  {
    //LOG("TCircularBuffer::Add\n");
    if (Count >= size) {
      LOG("TCircularBuffer::Add: no space left\n");
      return false;
    }
    Data[(Read + Count) % size] = t;
    Count++;
    return true;
  }

  /// Remove object from beginning of buffer.
  void Remove()
  {
    //LOG("TCircularBuffer::Remove\n");
    assert(Count > 0);
    Read = (Read + 1) % size;
    Count--;
  }

  /// Return object at beginning of buffer. Does not alter the buffer.
  const T& First() const
  {
    assert(Count > 0);
    return Data[Read];
  }

  /// Empty predicate.
  bool Empty() const
  {
    return Count == 0;
  }

private:
  static_assert(size < 256, "Too big buffer");
  T Data[size];
  uint8_t Read;
  uint8_t Count;
};
