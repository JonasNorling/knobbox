/* -*- c++ -*- */
#pragma once

#include <cstdint>

class TGui
{
public:
  TGui() :
    CurrentLine(0)
  { }

  void Process();
  void Update() { CurrentLine = 0; }

private:
  static const uint8_t Lines = 8;
  uint8_t CurrentLine;
};
