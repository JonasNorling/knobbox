#include "TGui.h"
#include "TPopup.h"

void TPopup::Render(uint8_t n, TDisplay::TPageBuffer* line)
{
  if (n == 0) {
    line->Clear(Margin, line->GetLength());
    char text[IDisplayPage::MenuTextLen];
    text[0] = '\0';
    Gui.GetCurrentPageObject()->GetMenuTitle(text);
    line->DrawText(text, Margin + TDisplay::GlyphWidth);
    line->Invert(Margin, line->GetLength());
  }
  else {
    const uint8_t item = Scroll + n-1;
    line->Clear(Margin, line->GetLength());
    char text[IDisplayPage::MenuTextLen + 2];
    text[0] = '\003';
    text[1] = '\0';

    if (item < ItemCount) {
      Gui.GetCurrentPageObject()->GetMenuItem(item, text + 1);

      if (text[1] == '\0') {
	// We discovered that this is past the end of the list.
	// If this item was selected, move selection up and repaint.
	ItemCount = item;
	if (Focus >= ItemCount) {
	  Focus = ItemCount - 1;
	  Gui.UpdateLine(n-1);
	}
      }

      line->DrawText(text, Margin);
      if (Focus == item) {
	line->Invert(Margin + TDisplay::GlyphWidth, line->GetLength());
      }
    }

    if (item >= ItemCount) {
      text[1] = '-';
      line->DrawText(text, Margin);
    }
  }
}

void TSelectPopup::Event(TEvent event)
{
  Gui.UpdateAll();
  switch (event_code(event)) {
  case KNOB_RIGHT:
  case KEY_UP:
    if (Focus > 0) {
      Focus--;
      if (Focus < Scroll) {
	Scroll = Focus;
      }
    }
    break;
  case KNOB_LEFT:
  case KEY_DOWN:
    if (Focus < ItemCount-1) {
      Focus++;
      if (Focus - Scroll >= Lines) {
	Scroll = Focus - Lines + 1;
      }
    }
    break;
  case KNOB_PUSH:
  case KEY_OK:
    Gui.GetCurrentPageObject()->MenuItemSelected(Focus);
    Gui.ChangeFocus(TGui::FOCUS_PAGE);
    break;
  case KEY_BACK:
    Gui.ChangeFocus(TGui::FOCUS_PAGE);
    break;
  default:
    break;
  }
}

void TSettingsPopup::Event(TEvent event)
{
  Gui.UpdateAll();
  switch (event_code(event)) {
  case KNOB_RIGHT:
  case KEY_UP:
    if (Selected) {
      Gui.GetCurrentPageObject()->MenuItemChanged(Focus, 1);
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
    if (Selected) {
      Gui.GetCurrentPageObject()->MenuItemChanged(Focus, -1);
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
    Selected = !Selected;
    if (Selected) {
      Gui.GetCurrentPageObject()->MenuItemSelected(Focus);
    }
    break;
  case KEY_BACK:
    Gui.ChangeFocus(TGui::FOCUS_PAGE);
    break;
  default:
    break;
  }
}
