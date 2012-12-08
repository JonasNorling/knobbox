/* -*- c++ -*- */
#pragma once

#include <cstdint>
#include "IDisplayPage.h"
#include "TFlashTest.h"
#include "TGui.h"

class TSettingsPage : public TDisplayPageBase
{
public:
  enum TFocus {
    // Currently focused field
    FOCUS_TOP_MENU, FOCUS_FLASHREAD,
    FOCUS_FLASHWRITE,
    FOCUS_LAST = FOCUS_FLASHWRITE
  };

  TSettingsPage() : Focus(FOCUS_TOP_MENU) { }
  void Render(uint8_t n, TDisplay::TPageBuffer* line);
  void Show();

private:
  TTopMenu TopMenu;

  uint8_t Focus;

  TFlashTest FlashTester;
};
