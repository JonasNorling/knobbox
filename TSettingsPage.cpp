#include "TSettingsPage.h"
#include "TGui.h"
#include "utils.h"
#include "TFlashTest.h"

void TSettingsPage::Render(uint8_t n, TDisplay::TPageBuffer* line)
{
  if (n == 1) {
    line->DrawText("Start flash read", 0, Focus == FOCUS_FLASHREAD);
  }
  else if (n == 2) {
    line->DrawText("Start flash write", 0, Focus == FOCUS_FLASHWRITE);
  }
}

void TSettingsPage::Event(TEvent event)
{
  switch (event_code(event)) {
  case RECEIVE_FOCUS:
    Focus = FOCUS_FLASHREAD;
    Gui.UpdateAll();
    break;
  case KEY_DOWN:
    if (Focus < FOCUS_LAST) {
      Focus++;
    }
    Gui.UpdateAll();
    break;
  case KEY_UP:
    Focus--;
    if (Focus == 0) {
      Gui.ChangeFocus(TGui::FOCUS_MENU);
    }
    Gui.UpdateAll();
    break;
  case KEY_OK:
    switch (Focus) {
    case FOCUS_FLASHREAD:
      FlashTester.RunRead();
      break;
    case FOCUS_FLASHWRITE:
      FlashTester.RunWrite();
      break;
    }
    break;
  }
}

