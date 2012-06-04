#include "TSeqPage.h"
#include "TSequencer.h"
#include "TGui.h"
#include "utils.h"

void TSeqPage::Render(uint8_t n, TDisplay::TPageBuffer* line)
{
  uint8_t pos = LeftMargin;
  if (n == 1) {
    line->DrawText("\020S1\021S2 S3 S4 all", 0);
    line->Invert(4*TDisplay::GlyphWidth, line->GetLength());
  }
  else if (n == 2) {
    pos = line->DrawText("setup\022", pos, Focus == FOCUS_SETUPMENU);
    pos = line->Advance(pos);
    pos = line->DrawText("actions\022", pos, Focus == FOCUS_ACTIONMENU);
  }
  else if (n == 3) {
    char text[9];
    cheap_strcpy(text, "step xx:");
    render_uint(text+5, Sequencer.ActiveStep + 1, 2);
    pos = line->DrawText(text, pos, Focus == FOCUS_STEP && !Blink);
    pos = line->Advance(pos);

    cheap_strcpy(text, "N=xxx");
    render_uint(text+2, Sequencer.Scenes[0].Data[Sequencer.ActiveStep].Note, 3);
    pos = line->DrawText(text, pos, Focus == FOCUS_NOTE);
  }
  else if (n == 4) {
    line->DrawText("action: start S2", LeftMargin, Focus == FOCUS_ACTION);
  }
  else if (n == 5) {
    pos = line->DrawText("velo 097", pos, Focus == FOCUS_VELO);
    pos = line->Advance(pos);
    pos = line->DrawText("len \033", pos, Focus == FOCUS_LEN);
  }
  else if (n == 6) {
    pos = line->DrawText("offs +0", pos, Focus == FOCUS_OFFSET);
    pos = line->Advance(pos);
    pos = line->Advance(pos);
    pos = line->DrawText("CC 034", pos, Focus == FOCUS_CC);
  }
  else if (n == 7) {
    char text[20];
    cheap_strcpy(text, "\033=     USB  MIDI");
    render_uint(text+2, Sequencer.GetTempo(), 3);
    line->DrawText(text, LeftMargin);
    line->Invert(0, line->GetLength());
  }
}

void TSeqPage::Event(TEvent event)
{
  /// \todo This will get a bit messy if we keep on this way. There's
  /// probably a splendid design pattern that I won't use...
  Gui.UpdateAll();
  switch (event) {
  case RECEIVE_FOCUS:
    Focus = FOCUS_SETUPMENU;
    break;
  case KEY_DOWN:
    if (Selected) {
      switch (Focus) {
      case FOCUS_STEP:
	Sequencer.ActiveStep--;
	break;
      }
    } else if (Focus < FOCUS_LAST) {
      Focus++;
    }
    break;
  case KEY_UP:
    if (Selected) {
      switch (Focus) {
      case FOCUS_STEP:
	Sequencer.ActiveStep++;
	break;
      }
    } else {
      Focus--;
      if (Focus == 0) {
	Gui.ChangeFocus(TGui::FOCUS_MENU);
      }
    }
    break;
  case KEY_OK:
    switch (Focus) {
    case FOCUS_SETUPMENU:
      Gui.ChangeFocus(TGui::FOCUS_POPUP);
      break;
    default:
      Selected = !Selected;
      Blink = Selected;
      break;
    }
    break;
  case KEY_BACK:
    break;
  case BLINK_TIMER:
    if (Selected) {
      Blink = !Blink;
      Gui.UpdateAll(); // A bit wasteful
    }
    break;
  default:
    break;
  }
}

void TSeqPage::GetMenuTitle(char text[MenuTextLen])
{
  switch (Focus) {
  case FOCUS_SETUPMENU: {
    cheap_strcpy(text, "Setup seq");
  }
  }
}

void TSeqPage::GetMenuItem(uint8_t n, char text[MenuTextLen])
{
  switch (Focus) {
  case FOCUS_SETUPMENU: {
    const int item_count = 6;
    const char* items[item_count] = { "MODE: lin 12",
				      "TEMPO: \033=120",
				      "INSTR: 7",
				      "CHANNEL: \03714",
				      "STEP: \033 1/4",
				      "CC: 14 Attack" };
    if (n >= item_count) return;
    cheap_strcpy(text, items[n]);
    break;
  }
  }
}

void TSeqPage::MenuItemSelected(uint8_t /*n*/)
{
}
