/* -*- c++ -*- */
#pragma once

#include <cstdint>
#include "IDisplayPage.h"

class TSeqPage : public IDisplayPage
{
public:
  TSeqPage() { }
  void Render(uint8_t n, TDisplay::TPageBuffer* line);
  void Event(TEvent event);
};
