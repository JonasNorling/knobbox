#include "TControllerPage.h"
#include "TGui.h"
#include "TSequencer.h"
#include "TControllers.h"
#include "utils.h"

void TControllerPage::Render(uint8_t n, TDisplay::TPageBuffer* line)
{
  const int currentKnob = Controllers.GetActiveKnob();
  const uint8_t currentValue = Controllers.GetValue(currentKnob);

  if (n == 1) {
    char channel[4];
    channel[0] = '\037';
    channel[1] = '0' + CurrentChannel + 1;
    channel[2] = ' ';
    channel[3] = '\0';
    uint8_t pos = line->DrawText(channel, LeftMargin, Focus == FOCUS_CHANNEL);
    line->DrawText(Controllers.GetScene().Name, pos, Focus == FOCUS_SET);
  }
  else if (n == 2) {
    line->DrawText("clock ON  seq ON", LeftMargin);
  }
  else if (n == 3) {
    const int barstart = 18;
    int i = -1;
    line->Data[barstart+i] = 0x7e;
    for (; i < currentValue / 2; i++) {
      line->Data[barstart+i] = 0x7e;
    }
    for (; i < 64; i++) {
      line->Data[barstart+i] = 0x42;
    }
    line->Data[barstart+i] = 0x7e;
  }
  else if (n == 4) {
    char text[20];
    cheap_strcpy(text, "\036knob xx: xxx");
    render_uint(text+6, currentKnob + 1, 2);
    render_uint(text+10, currentValue, 3);
    line->DrawText(text, LeftMargin);
  }
  else if (n == 5) {
    char text[20];
    cheap_strcpy(text, "\036\037xx ");
    render_uint(text+2, Controllers.GetScene().Knobs[currentKnob].Channel + 1, 2);
    cheap_strcpy(text+5, Controllers.GetScene().Knobs[currentKnob].InstrumentName);
    line->DrawText(text, LeftMargin);
  }
  else if (n == 6) {
    char text[20];
    cheap_strcpy(text, "\036CCxx ");
    render_uint(text+3, Controllers.GetScene().Knobs[currentKnob].Param, 2);
    cheap_strcpy(text+6, Controllers.GetScene().Knobs[currentKnob].Name);
    line->DrawText(text, LeftMargin);
  }
  else if (n == 7) {
    char text[20];
    cheap_strcpy(text, "Flash=");
    render_hexbyte(text + 6, Memory.Data()[0]);
    render_hexbyte(text + 8, Memory.Data()[1]);
    render_hexbyte(text +10, Memory.Data()[2]);
    render_hexbyte(text +12, Memory.Data()[3]);
    render_hexbyte(text +14, Memory.Data()[4]);
    text[16] = '\0';

    line->DrawText(text, LeftMargin);
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
    cheap_strcpy(text, "MIDI channel");
  }
  }
}

void TControllerPage::GetMenuItem(uint8_t n, char text[MenuTextLen])
{
  switch (Focus) {
  case FOCUS_CHANNEL: {
    if (n > 15) return;
    const char* channel = "Channel \037";
    size_t i = cheap_strcpy(text, channel);
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
