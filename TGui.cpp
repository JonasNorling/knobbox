#include "TGui.h"
#include "TDisplay.h"
#include "TBitmask.h"
#include "TSeqPage.h"
#include "TSettingsPage.h"
#include "TControllers.h"
#include "TSequencer.h"

/*
void TGui::DrawCharmap()
{
  if (CurrentLine < Lines) {
    TDisplay::TPageBuffer* line = Display.GetBuffer();
    if (line) {
      line->Clear();
      char str[17];
      uint8_t i;
      for (i = 0; i < sizeof(str)-1; i++) {
	str[i] = CurrentLine*16 + i;
      }
      str[i] = '\0';
      if (str[0] == '\0') str[0] = ' ';
      uint8_t offset = line->DrawText(str, 3);
      Display.OutputBuffer(line, offset, CurrentLine, 0);
      CurrentLine++;
    }
  }
}
*/

void TTopMenu::Render(uint8_t n __attribute__((unused)),
		      TDisplay::TPageBuffer* line)
{
  int pos = 3;
  const int selected = Mode;
  const char* strings[3] = {"CTRL", "SEQ", "SETUP"};
  int shadestart = 0;
  int shadeend = 0;

  for (int i = 0; i < 3; i++) {
    if (i == selected) {
      shadestart = pos;
      pos = line->DrawText("\020", pos);
    }
    pos = line->DrawText(strings[i], pos,
			 i == selected && Gui.GetFocus() == TGui::FOCUS_MENU);
    if (i == selected) {
      pos = line->DrawText("\021", pos);
      shadeend = pos;
    } else {
      pos = line->Advance(pos);
    }
  }

  line->Invert(0, shadestart);
  line->Invert(shadeend, line->GetLength());
}

void TTopMenu::Event(TEvent event)
{
  switch (event) {
  case KEY_OK:
    if (Mode != MODE_LAST) {
      Mode++;
      Gui.SetPage();
    }
    break;
  case KEY_BACK:
    if (Mode != MODE_FIRST) {
      Mode--;
      Gui.SetPage();
    }
    break;
  case KEY_DOWN:
    Gui.ChangeFocus(TGui::FOCUS_PAGE);
    break;
  default:
    break;
  }
}


TGui::TGui() :
  DirtyLines(TBitmask::Init(Lines)),
  Focus(FOCUS_MENU)
{
  SetPage();
}

void TGui::SetPage()
{
  switch (Mode) {
  case MODE_CONTROLLER:
    static_assert(sizeof(TControllerPage) <= sizeof(CurrentPageObject), "Too large object");
    new(CurrentPageObject) TControllerPage();
    Controllers.UpdateKnobs();
    break;
  case MODE_SEQ:
    static_assert(sizeof(TSeqPage) <= sizeof(CurrentPageObject), "Too large object");
    new(CurrentPageObject) TSeqPage();
    Sequencer.UpdateKnobs();
    break;
  case MODE_SETTINGS:
    static_assert(sizeof(TSettingsPage) <= sizeof(CurrentPageObject), "Too large object");
    new(CurrentPageObject) TSettingsPage();
    break;
  }
  UpdateAll();
}

void TGui::Process()
{
  int8_t n = TBitmask::FindSet(DirtyLines, Lines);
  if (n != -1) {
    TDisplay::TPageBuffer* line = Display.GetBuffer();
    if (line) {
      line->Clear();
      if (n == 0) {
	TopMenu.Render(n, line);
      } else {
	GetCurrentPageObject()->Render(n, line);
      }
      if (Focus == FOCUS_POPUP) {
	if (GetCurrentPopupObject()) {
	  GetCurrentPopupObject()->Render(n, line);
	}
      }
      Display.OutputBuffer(line, line->GetLength(), n, 0);
    } else {
      TBitmask::Set(DirtyLines, n);
    }
  }
}

void TGui::UpdateAll()
{
  DirtyLines = TBitmask::Init(Lines);
}

void TGui::ChangeFocus(TFocus focus)
{
  Focus = focus;
  switch (Focus) {
  case FOCUS_MENU:
    TopMenu.Event(RECEIVE_FOCUS);
    break;
  case FOCUS_PAGE:
    GetCurrentPageObject()->Event(RECEIVE_FOCUS);
    break;
  case FOCUS_POPUP:
    break;
  }
}
