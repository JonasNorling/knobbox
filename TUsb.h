/* -*- c++ -*- */
#pragma once

#include "device.h"

class TUsb
{
public:
  TUsb() { }
  void Init();

  void Poll() {
#ifndef HOST
    usbd_poll();
#endif
  }

private:
  static void SetConfig(uint16_t value);
};
