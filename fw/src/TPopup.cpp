#include "TGui.h"
#include "TPopup.h"
#include <cstring>

void TPopup::Render(uint8_t n, TDisplay::TPageBuffer* line)
{
    const int startx = 0;
    if (n == 0) {
        line->Clear(startx, line->GetLength());
        char text[MenuTextLen];
        text[0] = '\0';
        GetMenuTitle(text);
        line->DrawText(text, startx + TDisplay::GlyphWidth);
        line->Invert(startx, line->GetLength());
    }
    else {
        const uint8_t item = Scroll + n-1;
        line->Clear(startx, line->GetLength());
        char text[MenuTextLen + 2];
        text[0] = '\003';
        text[1] = '\0';

        if (item < ItemCount) {
            GetMenuItem(item, text + 1);

            if (text[1] == '\0') {
                // We discovered that this is past the end of the list.
                // If this item was selected, move selection up and repaint.
                ItemCount = item;
                if (Focus >= ItemCount) {
                    Focus = ItemCount - 1;
                    Gui.UpdateLine(n-1);
                }
            }

            line->DrawText(text, startx);
            if (Focus == item) {
                line->Invert(startx + TDisplay::GlyphWidth, line->GetLength());
            }
        }

        if (item >= ItemCount) {
            text[1] = '-';
            line->DrawText(text, startx);
        }
    }
}

void TNamePopup::SetName(const char* name)
{
    memset(Name, ' ', sizeof(Name));
    cheap_strncpy(Name, name, sizeof(Name));
    ItemCount = sizeof(Name);
}

void TNamePopup::Render(uint8_t n, TDisplay::TPageBuffer* line)
{
    const int startx = 0;
    if (n == 0) {
        line->Clear(startx, line->GetLength());
        line->DrawText("Enter name", startx + TDisplay::GlyphWidth);
        line->Invert(startx, line->GetLength());
    }
    else if (n == 3) {
        line->Clear(startx, line->GetLength());
        line->DrawText("\003\011\011\011\011\011\011\011\011\011\011\011\011", startx);
    }
    else if (n == 4) {
        line->Clear(startx, line->GetLength());
        char text[sizeof(Name) + 2];
        text[0] = '\003';
        text[1] = '\0';
        cheap_strncpy(text + 1, Name, sizeof(Name));
        line->DrawText(text, startx);

        // Highlight selected letter
        line->Invert(startx + TDisplay::GlyphWidth * (Focus + 1), startx + TDisplay::GlyphWidth * (Focus + 2));
    }
    else if (n == 5) {
        line->Clear(startx, line->GetLength());
        line->DrawText("\003\012\012\012\012\012\012\012\012\012\012\012\012", startx);
    }
    else {
        line->Clear(startx, line->GetLength());
        line->DrawText("\003", startx);
    }
}

bool TNamePopup::Show()
{
    Gui.UpdateAll();

    while (true) {
        TEvent event = Gui.WaitForEvent();

        switch (event_code(event)) {
        case RENDER_LINE: {
            const uint8_t n = event_value(event);
            TDisplay::TPageBuffer* line = Display.GetBuffer();
            if (line) {
                line->Clear();
                Render(n, line);
                Display.OutputBuffer(line, line->GetLength(), n, Margin);
            }
            else {
                Gui.UpdateLine(n); // Try again
            }
            break;
        }

        case KNOB_RIGHT:
        case KEY_DOWN:
            Gui.UpdateAll();
            if (Focus < ItemCount-1) {
                Focus++;
            }
            break;

        case KNOB_LEFT:
        case KEY_UP:
            Gui.UpdateAll();
            if (Focus > 0) {
                Focus--;
            }
            break;

        case KNOB_PUSH:
        case KEY_OK:
            return true;
            break;

        case KEY_BACK:
            return false;
            break;
        }
    }
    return false;
}

int TSelectPopup::Show(int initFocus)
{
    Focus = initFocus;

    if (Focus - Scroll >= Lines) {
        Scroll = Focus - Lines + 1;
    }

    Gui.UpdateAll();

    while (true) {
        TEvent event = Gui.WaitForEvent();

        switch (event_code(event)) {
        case RENDER_LINE: {
            const uint8_t n = event_value(event);
            TDisplay::TPageBuffer* line = Display.GetBuffer();
            if (line) {
                line->Clear();
                Render(n, line);
                Display.OutputBuffer(line, line->GetLength(), n, Margin);
            }
            else {
                Gui.UpdateLine(n); // Try again
            }
            break;
        }

        case KNOB_RIGHT:
        case KEY_UP:
            if (Focus > 0) {
                Gui.UpdateAll();
                Focus--;
                if (Focus < Scroll) {
                    Scroll = Focus;
                }
            }
            break;

        case KNOB_LEFT:
        case KEY_DOWN:
            if (Focus < ItemCount-1) {
                Gui.UpdateAll();
                Focus++;
                if (Focus - Scroll >= Lines) {
                    Scroll = Focus - Lines + 1;
                }
            }
            break;

        case KNOB_PUSH:
        case KEY_OK:
            return Focus;
            break;

        case KEY_BACK:
            return -1;
            break;
        }
    }

    return -1;
}

int TSettingsPopup::Show()
{
    Focus = 0;

    Gui.UpdateAll();

    while (true) {
        TEvent event = Gui.WaitForEvent();

        switch (event_code(event)) {
        case RENDER_LINE: {
            const uint8_t n = event_value(event);
            TDisplay::TPageBuffer* line = Display.GetBuffer();
            if (line) {
                line->Clear();
                Render(n, line);
                Display.OutputBuffer(line, line->GetLength(), n, Margin);
            }
            else {
                Gui.UpdateLine(n); // Try again
            }
            break;
        }

        case KNOB_RIGHT:
        case KEY_UP:
            Gui.UpdateAll();
            if (Selected) {
                ItemChanged(Focus, 1);
            }
            else {
                if (Focus > 0) {
                    Focus--;
                    if (Focus < Scroll) {
                        Scroll = Focus;
                    }
                }
            }
            break;

        case KNOB_LEFT:
        case KEY_DOWN:
            Gui.UpdateAll();
            if (Selected) {
                ItemChanged(Focus, -1);
            }
            else {
                if (Focus < ItemCount-1) {
                    Focus++;
                    if (Focus - Scroll >= Lines) {
                        Scroll = Focus - Lines + 1;
                    }
                }
            }
            break;

        case KNOB_PUSH:
        case KEY_OK:
            Gui.UpdateAll();
            Selected = !Selected;
            if (Selected) {
                bool ret = ItemSelected(Focus);
                if (ret) {
                    return Focus;
                }
            }
            break;

        case KEY_BACK:
            return -1;
            break;
        }
    }

    return -1;
}

