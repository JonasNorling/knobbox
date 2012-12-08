#include "TGui.h"
#include "TDisplay.h"
#include "TBitmask.h"
#include "TSeqPage.h"
#include "TSettingsPage.h"
#include "TControllerPage.h"
#include "TControllers.h"
#include "TSequencer.h"
#include "TScheduler.h"

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
		      TDisplay::TPageBuffer* line, bool haveFocus)
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
    pos = line->DrawText(strings[i], pos, i == selected && haveFocus);
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
    }
    break;
  case KEY_BACK:
    if (Mode != MODE_FIRST) {
      Mode--;
    }
    break;
  }
}


TGui::TGui() :
  DirtyLines(TBitmask::Init(Lines)),
  PendingEvent(NO_EVENT)
{
}

void TGui::Show()
{
    while (true) {
        switch (Mode) {
        case MODE_CONTROLLER: {
            TControllerPage page;
            page.Show();
            break;
        }
        case MODE_SEQ: {
            TSeqPage page;
            page.Show();
            break;
        }
        case MODE_SETTINGS: {
            TSettingsPage page;
            page.Show();
            break;
        }
        }
    }
}

TEvent TGui::WaitForEvent()
{
    TEvent event = PendingEvent;
    if (event != NO_EVENT) {
        PendingEvent = NO_EVENT;
        return event;
    }

    TScheduler::Yield();

    event = PendingEvent;
    if (event == NO_EVENT) {
        int8_t n = TBitmask::FindFree(DirtyLines, Lines);
        if (n >= 0) {
            event = construct_event(RENDER_LINE, n);
        }
    }
    PendingEvent = NO_EVENT;
    return event;
}
