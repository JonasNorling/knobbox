#include "TSeqPage.h"
#include "TSequencer.h"
#include "TGui.h"
#include "utils.h"

const TSeqPage::eventhandler_t TSeqPage::EventHandler[FOCUS_LAST + 1] = {
  0,
  0,
  0,
  &TSeqPage::EventHandlerStep,
  0,
  0,
  &TSeqPage::EventHandlerVelo,
  &TSeqPage::EventHandlerLen,
  &TSeqPage::EventHandlerOffset,
  0 };

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
    render_uint(text+5, CurrentStep + 1, 2);
    pos = line->DrawText(text, pos, Focus == FOCUS_STEP && !Blink);
    pos = line->Advance(pos);

    pos = line->DrawText(TSequencer::NoteName(Sequencer.Scenes[0].Data[CurrentStep].Note),
			 pos, Focus == FOCUS_NOTE);
  }
  else if (n == 4) {
    line->DrawText("action: start S2", LeftMargin, Focus == FOCUS_ACTION);
  }
  else if (n == 5) {
    char text[9];
    cheap_strcpy(text, "velo xxx");
    render_uint(text+5, Sequencer.Scenes[0].Data[CurrentStep].Velocity, 3);
    pos = line->DrawText(text, pos, Focus == FOCUS_VELO && !Blink);
    pos = line->Advance(pos);

    cheap_strcpy(text, "len    ");
    render_note_time(text+4, Sequencer.Scenes[0].Data[CurrentStep].Len);
    pos = line->DrawText(text, pos, Focus == FOCUS_LEN && !Blink);
  }
  else if (n == 6) {
    char text[9];
    cheap_strcpy(text, "ofs     ");
    render_signed_note_time(text+3, Sequencer.Scenes[0].Data[CurrentStep].Offset);
    pos = line->DrawText(text, pos, Focus == FOCUS_OFFSET && !Blink);

    pos = line->Advance(pos);
    pos = line->DrawText("CC 034", pos, Focus == FOCUS_CC && !Blink);
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
  Gui.UpdateAll();

  eventhandler_t fn = EventHandler[Focus];
  if (fn && (this->*fn)(event)) {
    return;
  }

  switch (event_code(event)) {
  case RECEIVE_FOCUS:
    Focus = FOCUS_SETUPMENU;
    break;
  case KEY_DOWN:
    if (Focus < FOCUS_LAST) {
      Focus++;
    }
    break;
  case KEY_UP:
    Focus--;
    if (Focus == 0) {
      Gui.ChangeFocus(TGui::FOCUS_MENU);
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
    }
    break;
  case KNOB_RIGHT:
    CurrentStep = event_value(event);
    Sequencer.ChangeNote(CurrentStep, 1);
    break;
  case KNOB_LEFT:
    CurrentStep = event_value(event);
    Sequencer.ChangeNote(CurrentStep, -1);
    break;
  case KNOB_PUSH:
    CurrentStep = event_value(event);
    Sequencer.ToggleEnable(CurrentStep);
    break;
  default:
    break;
  }
}

bool TSeqPage::EventHandlerStep(TEvent event)
{
  if (Selected) {
    switch (event_code(event)) {
    case KEY_DOWN:
      CurrentStep--;
      return true;
    case KEY_UP:
      CurrentStep++;
      return true;
    }
  }
  return false;
}

bool TSeqPage::EventHandlerVelo(TEvent event)
{
  if (Selected) {
    switch (event_code(event)) {
    case KNOB_LEFT:
      CurrentStep = event_value(event);
      // Fall-through
    case KEY_DOWN:
      Sequencer.ChangeVelocity(CurrentStep, -1);
      return true;

    case KNOB_RIGHT:
      CurrentStep = event_value(event);
      // Fall-through
    case KEY_UP:
      Sequencer.ChangeVelocity(CurrentStep, 1);
      return true;
    }
  }
  return false;
}

bool TSeqPage::EventHandlerLen(TEvent event)
{
  if (Selected) {
    switch (event_code(event)) {
    case KNOB_LEFT:
      CurrentStep = event_value(event);
      // Fall-through
    case KEY_DOWN:
      Sequencer.ChangeLength(CurrentStep, -1);
      return true;

    case KNOB_RIGHT:
      CurrentStep = event_value(event);
      // Fall-through
    case KEY_UP:
      Sequencer.ChangeLength(CurrentStep, 1);
      return true;
    }
  }
  return false;
}

bool TSeqPage::EventHandlerOffset(TEvent event)
{
  if (Selected) {
    switch (event_code(event)) {
    case KNOB_LEFT:
      CurrentStep = event_value(event);
      // Fall-through
    case KEY_DOWN:
      Sequencer.ChangeOffset(CurrentStep, -1);
      return true;

    case KNOB_RIGHT:
      CurrentStep = event_value(event);
      // Fall-through
    case KEY_UP:
      Sequencer.ChangeOffset(CurrentStep, 1);
      return true;
    }
  }
  return false;
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
