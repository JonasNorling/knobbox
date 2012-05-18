#include "TSeqPage.h"
#include "TSequencer.h"
#include "TGui.h"
#include "utils.h"

void TSeqPage::Render(uint8_t n, TDisplay::TPageBuffer* line)
{
  if (n == 1) {
    line->DrawText("\020S1\021S2 S3 S4 all", 0);
    line->Invert(4*TDisplay::GlyphWidth, line->GetLength());
  }
  else if (n == 2) {
    uint8_t pos = LeftMargin;
    pos = line->DrawText("setup\022", pos, Focus == FOCUS_SETUPMENU);
    pos = line->Advance(pos);
    pos = line->DrawText("actions\022", pos, Focus == FOCUS_ACTIONMENU);
  }
  else if (n == 3) {
    line->DrawText("step 03: Eb3\022", LeftMargin);
  }
  else if (n == 4) {
    line->DrawText("action: start S2", LeftMargin);
  }
  else if (n == 5) {
    line->DrawText("velo 097 len \035", LeftMargin);
  }
  else if (n == 6) {
    line->DrawText("offs +0   CC 034", LeftMargin);
  }
  else if (n == 7) {
    char text[20];
    cheap_strcpy(text, "\035=     USB  MIDI");
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
