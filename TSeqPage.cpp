#include "TSeqPage.h"
#include "TSequencer.h"
#include "TGui.h"
#include "utils.h"

void TSeqPage::Render(uint8_t n, TDisplay::TPageBuffer* line)
{
  if (n == 1) {
    
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
