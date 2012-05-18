/* -*- c++ -*- */
#pragma once

#include <cstdint>
#include "IDisplayPage.h"

class TControllerPage : public IDisplayPage
{
public:
  enum TFocus { FOCUS_NONE, FOCUS_CHANNEL, FOCUS_SET,
		FOCUS_LAST = FOCUS_SET };

  TControllerPage() : CurrentChannel(7), Focus(FOCUS_CHANNEL) { }
  void Render(uint8_t n, TDisplay::TPageBuffer* line);
  void Event(TEvent event);

  void GetMenuTitle(char text[MenuTextLen]);
  void GetMenuItem(uint8_t n, char text[MenuTextLen]);
  void MenuItemSelected(uint8_t n);

private:
  uint8_t CurrentChannel; // This shouldn't be here

  uint8_t Focus;
};
