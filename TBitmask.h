#pragma once

#include <cstdint>

#ifdef HOST
#include <cassert>
#else
#define assert(x)
#endif

class TBitmask
{
public:
  /* All positions are initialized to be "free" or "set" */
  static uint8_t Init(const uint8_t& size)
  {
    assert(size <= 8);
    return (1 << size) - 1;
  }

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

  static void Release(uint8_t& mask, const uint8_t& bit)
  {
    assert(!(mask & (1 << bit)));
    mask |= (1 << bit);
  }

  static int8_t FindSet(uint8_t& mask, const uint8_t& size)
  {
    return FindFree(mask, size);
  }

  static void Set(uint8_t& mask, const uint8_t& bit)
  {
    Release(mask, bit);
  }
};
