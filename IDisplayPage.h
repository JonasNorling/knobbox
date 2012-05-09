/* -*- c++ -*- */
#pragma once

#include "TDisplay.h"

enum TEvent { KEY_UP, KEY_DOWN, KEY_OK, KEY_BACK, RECEIVE_FOCUS };

class IDisplayPage
{
public:
  virtual void Render(uint8_t n, TDisplay::TPageBuffer* line) = 0;
  virtual void Event(TEvent event) = 0;

  static const uint8_t MenuTextLen = 13;
  // Called by TPopup when rendering a popup menu
  virtual void GetMenuTitle(char text[MenuTextLen] __attribute__((unused))) { }
  // Called by TPopup when rendering a popup menu. If n is the last entry, just return.
  virtual void GetMenuItem(uint8_t /* n */,
			   char text[MenuTextLen] __attribute__((unused))) { }
  // Called by TPopup when closing a popup menu
  virtual void MenuItemSelected(uint8_t /* n */) { }

  static const uint8_t LeftMargin = 3;
  static const uint8_t RightMargin = 3;
};
