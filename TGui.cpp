#include "TGui.h"
#include "TDisplay.h"
#include "TBitmask.h"

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
  uint8_t pos = 0;
  uint8_t shadestart = pos = line->DrawText("\020CTRL\021", pos);
  pos = line->DrawText(" SEQ ", pos);
  pos = line->DrawText(" SETUP", pos);
  line->Invert(shadestart, line->GetLength());

  if (Gui.GetFocus() == TGui::FOCUS_MENU) {
    line->Invert(1*6, 5*6);
  }
}

void TTopMenu::Event(TEvent event)
{
  switch (event) {
  case RECEIVE_FOCUS:
    break;
  case KEY_DOWN:
    Gui.ChangeFocus(TGui::FOCUS_PAGE);
  default:
    break;
  }
}


// FIXME: Temporary
const char* TControllerPage::CurrentSetName = "Blofeld set1";

void TControllerPage::Render(uint8_t n, TDisplay::TPageBuffer* line)
{
  if (n == 1) {
    char channel[4];
    channel[0] = '\037';
    channel[1] = '7';
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
    line->DrawText("clock ON, seq ON", LeftMargin);
    line->Invert(LeftMargin, LeftMargin+6*8);
  }
  else if (n == 3) {
    line->DrawText("\005knob3: 122", LeftMargin);
  }
  else if (n == 4) {
    line->DrawText("\007CC33 Attack", LeftMargin);
  }
  else if (n == 5) {
    line->DrawText("\005knob9: 39", LeftMargin);
  }
  else if (n == 6) {
    line->DrawText("\007NPRNxxxx: 114", LeftMargin);
  }
  else if (n == 7) {
    line->DrawText("tempo, i/o...", LeftMargin);
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
    //if (Focus < FOCUS_LAST) Focus++;
    Focus++;
    if (Focus > FOCUS_LAST) Gui.ChangeFocus(TGui::FOCUS_POPUP);
    break;
  default:
    break;
  }
}


void TPopup::Render(uint8_t n, TDisplay::TPageBuffer* line)
{
  if (n == 0) {
    line->Clear(Margin, line->GetLength());
    line->DrawText(" Select foobar", Margin);
    line->Invert(Margin, line->GetLength());
  } else {
    line->Clear(Margin, line->GetLength());
    line->DrawText("\003Junk", Margin);
  }
}

void TPopup::Event(TEvent event)
{
  (void)event;
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
	CurrentPage->Render(n, line);
      }
      if (Focus == FOCUS_POPUP) {
	Popup.Render(n, line);
      }
      Display.OutputBuffer(line, line->GetLength(), n, 0);
    } else {
      TBitmask::Set(DirtyLines, n);
    }
  }
}

void TGui::ChangeFocus(TFocus focus)
{
  Focus = focus;
  switch (Focus) {
  case FOCUS_MENU:
    TopMenu.Event(RECEIVE_FOCUS);
    break;
  case FOCUS_PAGE:
    CurrentPage->Event(RECEIVE_FOCUS);
    break;
  case FOCUS_POPUP:
    UpdateAll();
    break;
  }
}
