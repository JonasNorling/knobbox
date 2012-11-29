#include "TSeqPage.h"
#include "TSequencer.h"
#include "TGui.h"
#include "utils.h"

const TSeqPage::eventhandler_t TSeqPage::EventHandler[FOCUS_AUX_LAST + 1] = {
  0,
  0,
  0,
  &TSeqPage::EventHandlerStep,
  0,
  0,
  &TSeqPage::EventHandlerVelo,
  &TSeqPage::EventHandlerLen,
  &TSeqPage::EventHandlerOffset,
  0,
  &TSeqPage::EventHandlerTempo,
  0,
  0,
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
    pos = line->DrawText(Sequencer.Scenes[0].Name, pos);
    line->Invert(0, line->GetLength());
    /*
    char text[12];
    cheap_strcpy(text, "\033=xxx xx:xx");
    render_uint(text+2, Sequencer.GetTempo(), 3);
    render_uint(text+6, Sequencer.GetPosition(0).Step, 2);
    render_hexbyte(text+9, Sequencer.GetPosition(0).Minor);
    pos = line->DrawText(text, pos, Focus == FOCUS_TEMPO && !Blink);
    line->Invert(0, line->GetLength());
    */
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
      Gui.DisplayPopup<TSettingsPopup>();
      break;
    default:
      Selected = !Selected;
      Blink = Selected;
      break;
    }
    break;
  case KEY_BACK:
    Sequencer.ToggleRunning();
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

bool TSeqPage::EventHandlerTempo(TEvent event)
{
  if (Selected) {
    switch (event_code(event)) {
    case KNOB_LEFT:
    case KEY_DOWN:
      Sequencer.ChangeTempo(-1);
      return true;

    case KNOB_RIGHT:
    case KEY_UP:
      Sequencer.ChangeTempo(1);
      return true;
    }
  }
  return false;
}

void TSeqPage::GetMenuTitle(char text[MenuTextLen])
{
  switch (Focus) {
  case FOCUS_SETUPMENU:
    cheap_strcpy(text, "Setup seq");
    break;
  case FOCUS_SETUP_POPUP_LOAD_SLOT:
    cheap_strcpy(text, "Whence, sir?");
    break;
  case FOCUS_SETUP_POPUP_STORE_SLOT:
    cheap_strcpy(text, "Where, sir?");
    break;
  case FOCUS_SETUP_POPUP_STORE_SLOT_NAME:
    cheap_strcpy(text, "Name, sir?");
    break;
  }
}

void TSeqPage::GetMenuItem(uint8_t n, char text[MenuTextLen])
{
  switch (Focus) {
  case FOCUS_SETUPMENU: {
    switch (n) {
    case 0:
      cheap_strcpy(text, "STEPS: xx");
      render_uint(text+7, Sequencer.Scenes[0].Steps, 2);
      break;
    case 1:
      cheap_strcpy(text, "TEMPO: \033=xxx");
      render_uint(text+9, Sequencer.GetTempo(), 3);
      break;
    case 2:
      cheap_strcpy(text, "INSTR: x");
      break;
    case 3:
      cheap_strcpy(text, "CHANNEL: \037xx");
      render_uint(text+10, Sequencer.Scenes[0].Channel + 1, 2);
      break;
    case 4: {
      cheap_strcpy(text, "STEP: 1/xx   ");
      int step = Sequencer.GetStepLength();
      render_uint(text+8, step, 2);   
      switch (step) {
      case 1:  text[11] = '\035'; break;
      case 2:  text[11] = '\034'; break;
      case 3:  text[11] = '\034'; text[12] = '3'; break;
      case 4:  text[11] = '\033'; break;
      case 6:  text[11] = '\033'; text[12] = '3'; break;
      case 8:  text[11] = '\032'; break;
      case 12: text[11] = '\032'; text[12] = '3'; break;
      case 16: text[11] = '\031'; break;
      case 24: text[11] = '\031'; text[12] = '3'; break;
      case 32: text[11] = '\030'; break;
      case 48: text[11] = '\030'; text[12] = '3'; break;
      }
      break;
    }
    case 5:
      cheap_strcpy(text, "CC: 14 Attack");
      break;
    case 6:
      cheap_strcpy(text, "MODE: linear");
      break;
    case 7:
      cheap_strcpy(text, "LOAD...");
      break;
    case 8:
      cheap_strcpy(text, "STORE...");
      break;
    }
    break;
  }
  case FOCUS_SETUP_POPUP_LOAD_SLOT:
  case FOCUS_SETUP_POPUP_STORE_SLOT: {
    if (n < TMemory::SEQ_SCENE_COUNT) {
      const TNameList* l = reinterpret_cast<TNameList*>(Memory.GetCachedBlock());
      if (l->Magic == TNameList::MAGIC) {
	if (n < l->FetchedCount) {
	  render_uint(text, n, 2);
	  cheap_strncpy(text+2, l->Names[n],
			IDisplayPage::MenuTextLen-1);
	  break;
	}
      }
      cheap_strcpy(text, "Slot xxx");
      render_uint(text+5, n, 3);
    }
    break;
  }
  }
}

void TSeqPage::MenuItemSelected(uint8_t n)
{
  switch (Focus) {
  case FOCUS_SETUPMENU: {
    switch (n) {
    case 7:
      Focus = FOCUS_SETUP_POPUP_LOAD_SLOT;
      Memory.FetchNames((TMemory::BlockSize * TMemory::BLOCK_FIRST_SEQ_SCENE + offsetof(TSequencerScene, Name)),
			NAMELEN,
			TMemory::SEQ_SCENE_COUNT,
			TMemory::BlockSize,
			0 /* callback */);
      Gui.DisplayPopup<TSelectPopup>();
      break;
    case 8:
      Focus = FOCUS_SETUP_POPUP_STORE_SLOT;
      Memory.FetchNames((TMemory::BlockSize * TMemory::BLOCK_FIRST_SEQ_SCENE + offsetof(TSequencerScene, Name)),
			NAMELEN,
			TMemory::SEQ_SCENE_COUNT,
			TMemory::BlockSize,
			0 /* callback */);
      Gui.DisplayPopup<TSelectPopup>();
      break;
    }
    break;
  }
  case FOCUS_SETUP_POPUP_LOAD_SLOT: {
    Focus = FOCUS_SETUPMENU;
    Sequencer.LoadFromMemory(/*scene*/ 0, /*patchno*/ n);
    Gui.RemovePopup();
    break;
  }
  case FOCUS_SETUP_POPUP_STORE_SLOT: {
    Focus = FOCUS_SETUP_POPUP_STORE_SLOT_NAME;
    Gui.DisplayPopup<TNamePopup>();
    TNamePopup* popup = reinterpret_cast<TNamePopup*>(Gui.GetCurrentPopupObject());

    // Slot names will have been cached by the name list that is currently displaying
    const TNameList* l = reinterpret_cast<TNameList*>(Memory.GetCachedBlock());
    if (l->Magic == TNameList::MAGIC && n < l->FetchedCount) {
      popup->SetName(l->Names[n]);
    } else {
      popup->SetName("olle");
    }
    //Focus = FOCUS_SETUPMENU;
    //Sequencer.StoreInMemory(/*scene*/ 0, /*patchno*/ n);
    break;
  }
  }
}

void TSeqPage::MenuItemChanged(uint8_t n, int8_t value)
{
  switch (Focus) {
  case FOCUS_SETUPMENU: {
    switch (n) {
    case 0:
      Sequencer.ChangeSteps(value);
      break;
    case 1:
      Sequencer.ChangeTempo(value);
      break;
    case 3:
      Sequencer.Scenes[0].Channel = clamp(Sequencer.Scenes[0].Channel + value, 0, 15);
      break;
    case 4:
      Sequencer.ChangeStepLength(value);
      break;
    }
    break;
  }
  }
}
