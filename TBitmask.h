/* -*- c++ -*- */
#pragma once

#include <cstdint>

#ifdef HOST
#include <cassert>
#else
#define assert(x)
#endif

/**
 * Bitmask handling functions. Two viewpoints are provided, with
 * identical implementations.
 */
class TBitmask
{
public:
  /// All positions are initialized to be "free" or "set".
  static uint8_t Init(const uint8_t& size)
  {
    assert(size <= 8);
    return (1 << size) - 1;
  }

  /// Find the next free bit in the mask. Mark that bit as taken (not
  /// free).
  /// \return bit number, or -1 if no free bit was found.
  static int8_t FindFree(uint8_t& mask, const uint8_t& size)
  {
    for (uint8_t i = 0; i < size; i++) {
      const uint8_t bit = 1 << i;
      if (mask & bit) {
	mask &= ~(1 << i);
	return i;
      }
    }
    return -1;
  }

  /// Release a bit, so the FindFree will find it.
  static void Release(uint8_t& mask, const uint8_t& bit)
  {
    assert(!(mask & (1 << bit)));
    mask |= (1 << bit);
  }

  /// Same as FindFree.
  static int8_t FindSet(uint8_t& mask, const uint8_t& size)
  {
    return FindFree(mask, size);
  }

  /// Same as Release.
  static void Set(uint8_t& mask, const uint8_t& bit)
  {
    Release(mask, bit);
  }
};
