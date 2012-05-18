/* -*- c++ -*- */
#pragma once

#include <cstdint>
#include "IDisplayPage.h"

class TSeqPage : public IDisplayPage
{
public:
  enum TFocus { FOCUS_NONE, FOCUS_SETUPMENU, FOCUS_ACTIONMENU,
		FOCUS_STEP, FOCUS_NOTE, FOCUS_ACTION,
		FOCUS_VELO, FOCUS_LEN, FOCUS_OFFSET, FOCUS_CC,
		FOCUS_LAST = FOCUS_CC };

  TSeqPage() { }
  void Render(uint8_t n, TDisplay::TPageBuffer* line);
  void Event(TEvent event);

  void GetMenuTitle(char text[MenuTextLen]);
  void GetMenuItem(uint8_t n, char text[MenuTextLen]);
  void MenuItemSelected(uint8_t n);

private:
  uint8_t Focus;
};
