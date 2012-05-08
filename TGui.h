/* -*- c++ -*- */
#pragma once

#include <cstdint>
#include "TBitmask.h"
#include "TDisplay.h"

/*
 * TGui keeps stuff together. There is a TTopMenu that renders the
 * mode menu. It can be given focus. There is a set of IDisplayPage
 * instances that handle individual modes' displays (controller,
 * sequencer, settings, ...). They can receive focus and give it back
 * depending on key presses. IDisplayPages are asked to render
 * themselves to buffers, one line at a time. There is also TPopups
 * that cover a part of the screen and only render their part.
 */

enum TEvent { KEY_UP, KEY_DOWN, KEY_OK, KEY_BACK, RECEIVE_FOCUS };

class IDisplayPage
{
public:
  virtual void Render(uint8_t n, TDisplay::TPageBuffer* line) = 0;
  virtual void Event(TEvent event) = 0;

  static const uint8_t LeftMargin = 3;
  static const uint8_t RightMargin = 3;
};


class TControllerPage : public IDisplayPage
{
public:
  enum TFocus { FOCUS_NONE, FOCUS_CHANNEL, FOCUS_SET, FOCUS_LAST=FOCUS_SET };
  TControllerPage() { }
  inline void Render(uint8_t n, TDisplay::TPageBuffer* line);
  void Event(TEvent event);

private:
  static const char* CurrentSetName;
  static const uint8_t CurrentChannel = 7;
  uint8_t Focus;
};


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
  TPopup() { }
  void Render(uint8_t n, TDisplay::TPageBuffer* line);
  void Event(TEvent event);
private:
  static const uint8_t Margin = 13;
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

private:
  uint8_t DirtyLines;
  TFocus Focus;
  IDisplayPage* CurrentPage;

  TTopMenu TopMenu;
  TControllerPage ControllerPage;
  TPopup Popup;
};

extern TGui Gui;
