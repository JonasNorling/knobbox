/* -*- c++ -*- */
#pragma once

class IMidi
{
public:
  virtual void SendClockTick() = 0;
  virtual void SendEvent(uint8_t type, uint8_t arg1, uint8_t arg2) = 0;
};
