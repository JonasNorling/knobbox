#include "TSeqPage.h"
#include "TSequencer.h"
#include "TGui.h"
#include "utils.h"

const TSeqPage::eventhandler_t TSeqPage::EventHandler[FOCUS_LAST + 1] = {
        0,
        &TSeqPage::EventHandlerSetup,
        0,
        &TSeqPage::EventHandlerStep,
        0,
        0,
        &TSeqPage::EventHandlerVelo,
        &TSeqPage::EventHandlerLen,
        &TSeqPage::EventHandlerOffset,
        0,
        &TSeqPage::EventHandlerTempo,
};

void TSeqPage::Show()
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

        eventhandler_t fn = EventHandler[Focus];
        if (fn && (this->*fn)(event)) {
            continue;
        }

        switch (event_code(event)) {
        case KEY_DOWN:
            if (Focus < FOCUS_LAST) Focus++;
            break;

        case KEY_UP:
            if (Focus > FOCUS_TOP_MENU) Focus--;
            break;

        case KEY_OK:
            if (Focus == FOCUS_TOP_MENU) {
                TopMenu.Event(event);
                return;
            }
            Selected = !Selected;
            Blink = Selected;
            break;

        case KEY_BACK:
            if (Focus == FOCUS_TOP_MENU) {
                TopMenu.Event(event);
                return;
            }
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
        }
    }
}

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

bool TSeqPage::EventHandlerSetup(TEvent event)
{
    switch (event_code(event)) {
    case KEY_OK:
        // FIXME: Measure and reduce stack usage
        int selection = -1;
        {
            TSetupMenuPopup popup;
            selection = popup.Show();
            Gui.UpdateAll();
        }
        if (selection == 7 || selection == 8) {
            int slot = -1;
            {
                TMemorySlotPopup slotPopup("Select slot\022");
                slot = slotPopup.Show();
            }
            if (selection == 7 && slot != -1) { // LOAD...
                Sequencer.LoadFromMemory(0, slot);
            }
            else if (selection == 8 && slot != -1) { // STORE...
                TNamePopup namePopup;

                // Slot names will have been cached by the name list that is currently displaying
                const TNameList* l = reinterpret_cast<TNameList*>(Memory.GetCachedBlock());
                if (l->Magic == TNameList::MAGIC && slot < l->FetchedCount) {
                    namePopup.SetName(l->Names[slot]);
                } else {
                    namePopup.SetName("no name");
                }
                if (namePopup.Show()) {
                    Sequencer.StoreInMemory(0, slot);
                }
            }
        }
        return true;
    }
    return false;
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

void TSetupMenuPopup::GetMenuTitle(char text[MenuTextLen])
{
    cheap_strcpy(text, "Setup seq");
}

void TSetupMenuPopup::GetMenuItem(uint8_t n, char text[MenuTextLen])
{
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
}

void TSetupMenuPopup::ItemChanged(uint8_t n, int8_t value)
{
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
}

bool TSetupMenuPopup::ItemSelected(uint8_t n)
{
    switch (n) {
    case 7: // LOAD...
    case 8: // STORE...
        return true;
    }
    return false;
}

TMemorySlotPopup::TMemorySlotPopup(const char* title) :
                        Title(title)
{
    // FIXME: Stack usage?
    Memory.FetchNames((TMemory::BlockSize * TMemory::BLOCK_FIRST_SEQ_SCENE + offsetof(TSequencerScene, Name)),
            NAMELEN,
            TMemory::SEQ_SCENE_COUNT,
            TMemory::BlockSize,
            0);
}

void TMemorySlotPopup::GetMenuTitle(char text[MenuTextLen])
{
    cheap_strcpy(text, Title);
}

void TMemorySlotPopup::GetMenuItem(uint8_t n, char text[MenuTextLen])
{
    if (n < TMemory::SEQ_SCENE_COUNT) {
        const TNameList* l = reinterpret_cast<TNameList*>(Memory.GetCachedBlock());
        if (l->Magic == TNameList::MAGIC) {
            if (n < l->FetchedCount) {
                render_uint(text, n, 2);
                cheap_strncpy(text+2, l->Names[n], MenuTextLen-1);
                return;
            }
        }
        cheap_strcpy(text, "Slot xxx");
        render_uint(text+5, n, 3);
    }
}
