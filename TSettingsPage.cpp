#include "TSettingsPage.h"
#include "TGui.h"
#include "utils.h"
#include "TFlashTest.h"

void TSettingsPage::Render(uint8_t n, TDisplay::TPageBuffer* line)
{
  if (n == 1) {
    line->DrawText("Start flash read", 0, Focus == FOCUS_FLASHREAD);
  }
  else if (n == 2) {
    line->DrawText("Start flash write", 0, Focus == FOCUS_FLASHWRITE);
  }
}

void TSettingsPage::Show()
{
    Gui.UpdateAll();

    while (true) {
        TEvent event = Gui.WaitForEvent();

        if (event_code(event) == RENDER_LINE) {
            const uint8_t n = event_value(event);
            TDisplay::TPageBuffer* line = Display.GetBuffer();
            if (line) {
                line->Clear();
                if (n == 0) {
                    TopMenu.Render(n, line, Focus == FOCUS_TOP_MENU);
                }
                else {
                    Render(n, line);
                }
                Display.OutputBuffer(line, line->GetLength(), n, 0);
            }
            else {
                Gui.UpdateLine(n); // Try again
            }
            continue;
        }

        Gui.UpdateAll();

        switch (event_code(event)) {
        case KEY_DOWN:
            if (Focus < FOCUS_LAST) Focus++;
            break;

        case KEY_UP:
            if (Focus > FOCUS_TOP_MENU) Focus--;
            break;

        case KEY_OK:
            switch (Focus) {
            case FOCUS_TOP_MENU:
                TopMenu.Event(event);
                return;
            case FOCUS_FLASHREAD:
                FlashTester.RunRead();
                break;
            case FOCUS_FLASHWRITE:
                FlashTester.RunWrite();
                break;
            }
            break;

        case KEY_BACK:
            switch (Focus) {
            case FOCUS_TOP_MENU:
                TopMenu.Event(event);
                return;
            }
            break;
        }
    }
}

