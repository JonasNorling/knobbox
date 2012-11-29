/* -*- c++ -*- */
#pragma once

#include <cstdint>
#include "TBitmask.h"
#include "TDisplay.h"
#include "TControllerPage.h"
#include "TSettingsPage.h"
#include "TPopup.h"

class TTopMenu
{
public:
  TTopMenu() { }
  void Render(uint8_t n, TDisplay::TPageBuffer* line);
  void Event(TEvent event);
};


/**
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
 */
class TGui
{
public:
  static const int Lines = 8;
  enum TFocus { FOCUS_MENU, FOCUS_PAGE, FOCUS_POPUP };
  TGui();

  void Process();
  void UpdateAll();
  void UpdateLine(uint8_t n) { TBitmask::Set(DirtyLines, n); }
  void Event(TEvent event)
  {
    switch (Focus) {
    case FOCUS_MENU: TopMenu.Event(event); break;
    case FOCUS_PAGE: GetCurrentPageObject()->Event(event); break;
    case FOCUS_POPUP: GetCurrentPopupObject()->Event(event); break;
    }
  }
  template<typename T> void DisplayPopup();
  void RemovePopup() { ChangeFocus(FOCUS_PAGE); }
  void ChangeFocus(TFocus focus);
  TFocus GetFocus() const { return Focus; }
  IDisplayPage* GetCurrentPageObject() {
    return reinterpret_cast<IDisplayPage*>(CurrentPageObject);
  }
  TPopup* GetCurrentPopupObject() {
    return reinterpret_cast<TPopup*>(CurrentPopupObject);
  }

  void SetPage();

private:
  uint8_t DirtyLines;
  TFocus Focus;

  TTopMenu TopMenu;
  // The current IDisplayPage implementation is put here with placement new. Delete is never called.
  uint8_t CurrentPageObject[sizeof(TSettingsPage)];
  uint8_t CurrentPopupObject[sizeof(TNamePopup)];
};

extern TGui Gui;

template<typename T> void TGui::DisplayPopup()
{
  static_assert(sizeof(T) <= sizeof(CurrentPopupObject), "Too large object");
  Focus = FOCUS_POPUP;
  new(CurrentPopupObject) T();
}
