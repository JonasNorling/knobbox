/* -*- c++ -*- */
#pragma once

#include "TDisplay.h"
#include "TEvent.h"

/**
 * A user interface main page. Handles render requests and input
 * events.
 *
 * Status: Implemented and works. Popup menus not thought through.
 */
class IDisplayPage
{
public:
  static const uint8_t MenuTextLen = 13;
  static const uint8_t LeftMargin = 3;
  static const uint8_t RightMargin = 3;

  virtual void Render(uint8_t n, TDisplay::TPageBuffer* line) = 0;
  virtual void Event(TEvent event) = 0;

  /// Called by TPopup when rendering a popup menu
  virtual void GetMenuTitle(char text[MenuTextLen] __attribute__((unused))) { }
  /// Called by TPopup when rendering a popup menu. If n is the last entry, just return.
  virtual void GetMenuItem(uint8_t /* n */,
			   char text[MenuTextLen] __attribute__((unused))) { }
  /// Called by TPopup when an item was selected.
  virtual void MenuItemSelected(uint8_t /* n */) { }
  /// Called by TPopup when an item was changed.
  virtual void MenuItemChanged(uint8_t /* n */, int8_t /* value */) { }
};
