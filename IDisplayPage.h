/* -*- c++ -*- */
#pragma once

#include "TDisplay.h"
#include "TEvent.h"

class TDisplayPageBase
{
public:
  static const uint8_t MenuTextLen = 13;
  static const uint8_t LeftMargin = 3;
  static const uint8_t RightMargin = 3;

  void Show();
  void Render(uint8_t n, TDisplay::TPageBuffer* line);
};
