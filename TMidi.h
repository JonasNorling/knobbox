/* -*- c++ -*- */
#pragma once
#include "device.h"

class TMidi
{
public:
  void SendClockTick() // 0xF8
  {
#ifndef HOST
    usart_send(USART2, 0xf8);
#endif
  }
};

extern TMidi Midi;
