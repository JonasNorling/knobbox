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
    line->DrawText("mode: 12 linear", LeftMargin);
  }
  else if (n == 3) {
    line->DrawText("<trigger magic>", LeftMargin);
  }
  else if (n == 4) {
    line->DrawText("step 3  note Eb3", LeftMargin);
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
  case KEY_UP:
    Gui.ChangeFocus(TGui::FOCUS_MENU);
    break;
  default:
    break;
  }
}
