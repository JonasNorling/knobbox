#include "TControllerPage.h"
#include "TGui.h"

// FIXME: Temporary
const char* TControllerPage::CurrentSetName = "Blofeld set1";

void TControllerPage::Render(uint8_t n, TDisplay::TPageBuffer* line)
{
  if (n == 1) {
    char channel[4];
    channel[0] = '\037';
    channel[1] = '0' + CurrentChannel + 1;
    channel[2] = ' ';
    channel[3] = '\0';
    uint8_t pos = line->DrawText(channel, LeftMargin);
    if (Focus == FOCUS_CHANNEL) {
      line->Invert(LeftMargin, pos);
    }
    line->DrawText(CurrentSetName, pos);
    if (Focus == FOCUS_SET) {
      line->Invert(pos, line->GetLength()-RightMargin);
    }
  }
  else if (n == 2) {
    line->DrawText("clock ON  seq ON", LeftMargin);
  }
  else if (n == 3) {
  }
  else if (n == 4) {
    line->DrawText("\005knob3: 122", LeftMargin);
  }
  else if (n == 5) {
    line->DrawText("\007CC33 Attack", LeftMargin);
  }
  else if (n == 6) {
  }
  else if (n == 7) {
    line->DrawText("\035=120  USB  MIDI", LeftMargin);
    line->Invert(0, line->GetLength());
  }
}

void TControllerPage::Event(TEvent event)
{
  Gui.UpdateAll();
  switch (event) {
  case RECEIVE_FOCUS:
    Focus = FOCUS_CHANNEL;
    break;
  case KEY_DOWN:
    if (Focus < FOCUS_LAST) Focus++;
    break;
  case KEY_UP:
    Focus--;
    if (Focus == 0) Gui.ChangeFocus(TGui::FOCUS_MENU);
    break;
  case KEY_OK:
    Gui.ChangeFocus(TGui::FOCUS_POPUP);
    break;
  default:
    break;
  }
}

void TControllerPage::GetMenuTitle(char text[MenuTextLen])
{
  switch (Focus) {
  case FOCUS_CHANNEL: {
    const char* title = "MIDI channel";
    int i = 0;
    for (; title[i] != '\0'; i++) text[i] = title[i];
    text[i] = '\0';
  }
  }
}

void TControllerPage::GetMenuItem(uint8_t n, char text[MenuTextLen])
{
  switch (Focus) {
  case FOCUS_CHANNEL: {
    if (n > 15) return;
    const char* channel = "Channel \037";
    int i = 0;
    for (; channel[i] != '\0'; i++) text[i] = channel[i];
    text[i++] = ' ';
    text[i++] = '0' + n + 1;
    text[i++] = '\0';
    break;
  }
  }
}

void TControllerPage::MenuItemSelected(uint8_t n)
{
  CurrentChannel = n;
}
