/* -*- c++ -*- */
#pragma once

#include <cstdint>
#include "IDisplayPage.h"

class TSeqPage : public IDisplayPage
{
public:
  enum TFocus {
    // Currently focused field
    FOCUS_NONE, FOCUS_SETUPMENU, FOCUS_ACTIONMENU,
    FOCUS_STEP, FOCUS_NOTE, FOCUS_ACTION,
    FOCUS_VELO, FOCUS_LEN, FOCUS_OFFSET, FOCUS_CC,
    FOCUS_TEMPO,
    FOCUS_LAST = FOCUS_TEMPO,

    // Current focus in auxilliary menues and stuff
    FOCUS_LOAD_SLOT,
    FOCUS_STORE_SLOT
  };

  TSeqPage() : Focus(FOCUS_NONE), CurrentStep(0),
	       Selected(false), Blink(false) { }
  void Render(uint8_t n, TDisplay::TPageBuffer* line);
  void Event(TEvent event);

  void GetMenuTitle(char text[MenuTextLen]);
  void GetMenuItem(uint8_t n, char text[MenuTextLen]);
  void MenuItemSelected(uint8_t n);
  void MenuItemChanged(uint8_t n, int8_t value);

private:
  uint8_t Focus;
  uint8_t CurrentStep;
  bool Selected; /// Focused item is selected and blinking
  bool Blink;

  // Event handlers for the current focus, should return true if the
  // event was consumed.
  typedef bool (TSeqPage::*eventhandler_t) (TEvent);
  const static eventhandler_t EventHandler[FOCUS_LAST + 1];

  bool EventHandlerStep(TEvent event);
  bool EventHandlerVelo(TEvent event);
  bool EventHandlerLen(TEvent event);
  bool EventHandlerOffset(TEvent event);
  bool EventHandlerTempo(TEvent event);
};
