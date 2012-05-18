#include "TSeqPage.h"
#include "TSequencer.h"
#include "TGui.h"
#include "utils.h"

void TSeqPage::Render(uint8_t n, TDisplay::TPageBuffer* line)
{
  uint8_t pos = LeftMargin;
  if (n == 1) {
    line->DrawText("\020S1\021S2 S3 S4 all", 0);
    line->Invert(4*TDisplay::GlyphWidth, line->GetLength());
  }
  else if (n == 2) {
    pos = line->DrawText("setup\022", pos, Focus == FOCUS_SETUPMENU);
    pos = line->Advance(pos);
    pos = line->DrawText("actions\022", pos, Focus == FOCUS_ACTIONMENU);
  }
  else if (n == 3) {
    pos = line->DrawText("step 03:", pos, Focus == FOCUS_STEP);
    pos = line->Advance(pos);
    pos = line->DrawText("Eb3\022", pos, Focus == FOCUS_NOTE);
  }
  else if (n == 4) {
    line->DrawText("action: start S2", LeftMargin, Focus == FOCUS_ACTION);
  }
  else if (n == 5) {
    pos = line->DrawText("velo 097", pos, Focus == FOCUS_VELO);
    pos = line->Advance(pos);
    pos = line->DrawText("len \033", pos, Focus == FOCUS_LEN);
  }
  else if (n == 6) {
    pos = line->DrawText("offs +0", pos, Focus == FOCUS_OFFSET);
    pos = line->Advance(pos);
    pos = line->Advance(pos);
    pos = line->DrawText("CC 034", pos, Focus == FOCUS_CC);
  }
  else if (n == 7) {
    char text[20];
    cheap_strcpy(text, "\033=     USB  MIDI");
    render_uint(text+2, Sequencer.GetTempo(), 3);
    line->DrawText(text, LeftMargin);
    line->Invert(0, line->GetLength());
  }
}

void TSeqPage::Event(TEvent event)
{
  Gui.UpdateAll();
  switch (event) {
  case RECEIVE_FOCUS:
    Focus = FOCUS_SETUPMENU;
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

void TSeqPage::GetMenuTitle(char text[MenuTextLen])
{
  switch (Focus) {
  case FOCUS_SETUPMENU: {
    cheap_strcpy(text, "Setup seq");
  }
  }
}

void TSeqPage::GetMenuItem(uint8_t n, char text[MenuTextLen])
{
  switch (Focus) {
  case FOCUS_SETUPMENU: {
    const int item_count = 6;
    const char* items[item_count] = { "MODE: lin 12",
				      "TEMPO: \033=120",
				      "INSTR: 7",
				      "CHANNEL: \03714",
				      "STEP: \033 1/4",
				      "CC: 14 Attack" };
    if (n >= item_count) return;
    cheap_strcpy(text, items[n]);
    break;
  }
  }
}

void TSeqPage::MenuItemSelected(uint8_t /*n*/)
{
}
