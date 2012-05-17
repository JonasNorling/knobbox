#include "TSeqPage.h"
#include "TGui.h"

void TSeqPage::Render(uint8_t n, TDisplay::TPageBuffer* line)
{
  (void)n;
  (void)line;
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
