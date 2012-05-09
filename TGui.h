/* -*- c++ -*- */
#pragma once

#include <cstdint>
#include "TBitmask.h"
#include "TDisplay.h"
#include "TControllerPage.h"

/*
 * TGui keeps stuff together. There is a TTopMenu that renders the
 * mode menu. It can be given focus. There is a set of IDisplayPage
 * instances that handle individual modes' displays (controller,
 * sequencer, settings, ...). They can receive focus and give it back
 * depending on key presses. IDisplayPages are asked to render
 * themselves to buffers, one line at a time. There is also TPopups
 * that cover a part of the screen and only render their part.
 *
 * Keep the active page in a piece of memory (union?) where it's
 * placement newed. TGui keeps a IDisplayPage* pointer to that address
 * and the TPopupMenu can make calls to the IDisplayPage when asking
 * for items and when an item has been selected.
 *
 * We could do the same with popups: we'll need different kinds, such
 * as menu (list) and ok/cancel type boxes.
 */


class TTopMenu
{
public:
  TTopMenu() { }
  void Render(uint8_t n, TDisplay::TPageBuffer* line);
  void Event(TEvent event);
};


class TPopup
{
public:
  TPopup() : Focus(0) { }
  void Render(uint8_t n, TDisplay::TPageBuffer* line);
  void Event(TEvent event);

private:
  static const uint8_t Margin = 13;
  uint8_t Focus;
};

class TGui
{
public:
  static const int Lines = 8;
  enum TFocus { FOCUS_MENU, FOCUS_PAGE, FOCUS_POPUP };

  TGui() :
    DirtyLines(TBitmask::Init(Lines)),
    Focus(FOCUS_MENU),
    CurrentPage(&ControllerPage)
  { }

  void Process();
  void UpdateAll() { DirtyLines = TBitmask::Init(Lines); }
  void UpdateLine(uint8_t n) { TBitmask::Set(DirtyLines, n); }
  void KeyEvent(TEvent event)
  {
    switch (Focus) {
    case FOCUS_MENU: TopMenu.Event(event); break;
    case FOCUS_PAGE: CurrentPage->Event(event); break;
    case FOCUS_POPUP: Popup.Event(event); break;
    }
  }
  void ChangeFocus(TFocus focus);
  TFocus GetFocus() const { return Focus; }
  IDisplayPage* GetCurrentPage() { return &ControllerPage; }

private:
  uint8_t DirtyLines;
  TFocus Focus;
  IDisplayPage* CurrentPage;

  TTopMenu TopMenu;
  TControllerPage ControllerPage;
  TPopup Popup;
};

extern TGui Gui;
