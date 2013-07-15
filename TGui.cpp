#include "TGui.h"
#include "TDisplay.h"
#include "TBitmask.h"
#include "TSeqPage.h"
#include "TSettingsPage.h"
#include "TControllerPage.h"
#include "TControllers.h"
#include "TSequencer.h"
#include "TScheduler.h"
#include "TStartAnimation.h"
#include "TLeds.h"

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
    // This method is the main loop in the GUI thread.

    {
        TStartAnimation anim;
        anim.Show();
    }

    int lastMode = -1;
    while (true) {

        switch (Mode) {
        case MODE_CONTROLLER: {
            TControllerPage page;
            page.Show();
            lastMode = MODE_CONTROLLER;
            break;
        }
        case MODE_SEQ: {
            if (TSeqPage::LastSubpageWasOverview()) {
                TSeqOverviewPage page(lastMode == MODE_SEQ ? TSeqOverviewPage::FOCUS_SCENE_MENU : TSeqOverviewPage::FOCUS_TOP_MENU);
                page.Show();
            }
            else {
                TSeqPage page(lastMode == MODE_SEQ ? TSeqPage::FOCUS_SCENE_MENU : TSeqPage::FOCUS_TOP_MENU);
                page.Show();
            }
            lastMode = MODE_SEQ;
            break;
        }
        case MODE_SETTINGS: {
            TSettingsPage page;
            page.Show();
            lastMode = MODE_SETTINGS;
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

    TLeds::Set(TLeds::LED_TP16, false);

    if (HaveDirtyLines()) {
        // If we have things to output, return here to poll Display.GetBuffer() insanely often
        TScheduler::Yield();
    }
    else {
        TScheduler::Block();
    }

    TLeds::Set(TLeds::LED_TP16, true);

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
