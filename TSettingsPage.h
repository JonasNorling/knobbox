/* -*- c++ -*- */
#pragma once

#include <cstdint>
#include "IDisplayPage.h"
#include "TFlashTest.h"

class TSettingsPage : public IDisplayPage
{
public:
  enum TFocus {
    // Currently focused field
    FOCUS_NONE, FOCUS_FLASHREAD,
    FOCUS_FLASHWRITE,
    FOCUS_LAST = FOCUS_FLASHWRITE
  };

  TSettingsPage() : Focus(FOCUS_NONE) { }
  void Render(uint8_t n, TDisplay::TPageBuffer* line);
  void Event(TEvent event);

private:
  uint8_t Focus;

  TFlashTest FlashTester;
};
