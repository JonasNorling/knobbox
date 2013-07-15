#include "TSeqPage.h"
#include "TSequencer.h"
#include "TGui.h"
#include "utils.h"

uint8_t TSeqPage::LastSubpage = 0;

const TSeqPage::eventhandler_t TSeqPage::EventHandler[FOCUS_LAST + 1] = {
        0,
        &TSeqPage::EventHandlerScene,
        &TSeqPage::EventHandlerSetup,
        &TSeqPage::EventHandlerStep,
        0,
        &TSeqPage::EventHandlerAction,
        &TSeqPage::EventHandlerVelo,
        &TSeqPage::EventHandlerLen,
        &TSeqPage::EventHandlerOffset,
        0,
        &TSeqPage::EventHandlerTempo,
};

void TSeqPage::Show()
{
    Gui.UpdateAll();
    Sequencer.UpdateKnobs();

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
            if (LastSubpage == TSequencer::SceneCount) {
                // Let the caller show the overview page
                return;
            }
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

        case KEY_LONGPRESS_BACK:
            Sequencer.Stop();
            Sequencer.Resync();
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
    const uint8_t sceneno = Sequencer.GetCurrentScene();

    if (n == 1) {
        int pos = 0;
        int shadestart = 0;
        int shadeend = 0;

        for (int i = 0; i < 4; i++) {
            if (i == sceneno) {
                shadestart = pos;
                pos = line->DrawText("\020", pos);
            }
            char str[5];
            str[0] = '0' + i;
            str[1] = Sequencer.Scenes[i].Flags & TSequencerScene::FLAG_ENABLED ? '\013' : '\014';
            str[2] = '\0';
            pos = line->DrawText(str, pos, i == sceneno && Focus == FOCUS_SCENE_MENU);
            if (i == sceneno) {
                pos = line->DrawText("\021", pos);
                shadeend = pos;
            } else {
                pos = line->Advance(pos);
            }
        }

        pos = line->DrawText("all", pos);

        line->Invert(0, shadestart);
        line->Invert(shadeend, line->GetLength());
    }
    else if (n == 2) {
        pos = line->DrawText("setup\022", pos, Focus == FOCUS_SETUPMENU);
    }
    else if (n == 3) {
        char text[9];
        cheap_strcpy(text, "step xx:");
        render_uint(text+5, CurrentStep + 1, 2);
        pos = line->DrawText(text, pos, Focus == FOCUS_STEP && !Blink);
        pos = line->Advance(pos);

        pos = line->DrawText(TSequencer::NoteName(Sequencer.Scenes[sceneno].Data[CurrentStep].Note),
                pos, Focus == FOCUS_NOTE);
    }
    else if (n == 4) {
        line->DrawText("act: start S2", LeftMargin, Focus == FOCUS_ACTION);
    }
    else if (n == 5) {
        char text[9];
        cheap_strcpy(text, "velo xxx");
        render_uint(text+5, Sequencer.Scenes[sceneno].Data[CurrentStep].Velocity, 3);
        pos = line->DrawText(text, pos, Focus == FOCUS_VELO && !Blink);
        pos = line->Advance(pos);

        cheap_strcpy(text, "len    ");
        render_note_time(text+4, Sequencer.Scenes[sceneno].Data[CurrentStep].Len);
        pos = line->DrawText(text, pos, Focus == FOCUS_LEN && !Blink);
    }
    else if (n == 6) {
        char text[9];
        cheap_strcpy(text, "ofs     ");
        render_signed_note_time(text+3, Sequencer.Scenes[sceneno].Data[CurrentStep].Offset);
        pos = line->DrawText(text, pos, Focus == FOCUS_OFFSET && !Blink);

        pos = line->Advance(pos);
        pos = line->DrawText("CC 034", pos, Focus == FOCUS_CC && !Blink);
    }
    else if (n == 7) {
        char text[12];
        cheap_strcpy(text, "\033=xxx xx:xx");
        render_uint(text+2, Sequencer.GetTempo(), 3);
        render_uint(text+6, Sequencer.GetPosition(sceneno).Step, 2);
        render_hexbyte(text+9, Sequencer.GetPosition(sceneno).Minor);
        pos = line->DrawText(text, pos, Focus == FOCUS_TEMPO && !Blink);
        line->Invert(0, line->GetLength());
    }
}

bool TSeqPage::EventHandlerScene(TEvent event)
{
    switch (event_code(event)) {
    case KNOB_PUSH:
        Sequencer.ToggleSceneEnable(event_value(event));
        return true;
        break;

    case KEY_OK:
        if (Sequencer.GetCurrentScene() < TSequencer::SceneCount - 1) {
            Sequencer.SetCurrentScene(Sequencer.GetCurrentScene() + 1);
            LastSubpage = Sequencer.GetCurrentScene();
        } else {
            // Overview page
            LastSubpage = TSequencer::SceneCount;
        }
        return true;
        break;

    case KEY_BACK:
        if (Sequencer.GetCurrentScene() > 0) {
            Sequencer.SetCurrentScene(Sequencer.GetCurrentScene() - 1);
            LastSubpage = Sequencer.GetCurrentScene();
        }
        return true;
        break;
    }
    return false;
}

bool TSeqPage::EventHandlerSetup(TEvent event)
{
    switch (event_code(event)) {
    case KEY_OK:
        TSetupMenuPopup::Popup();
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

bool TSeqPage::EventHandlerAction(TEvent event)
{
    switch (event_code(event)) {
    case KEY_OK:
        TActionMenuPopup::Popup();
        return true;
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

/*
 * ******************************************
 */

void TSeqOverviewPage::Show()
{
    Gui.UpdateAll();
    Sequencer.UpdateKnobs();

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
            if (Focus >= FOCUS_SCENE_1 && Focus <= FOCUS_SCENE_4) {
                Sequencer.SetCurrentScene(Focus - FOCUS_SCENE_1);
            }
            break;

        case KEY_UP:
            if (Focus > FOCUS_TOP_MENU) Focus--;
            if (Focus >= FOCUS_SCENE_1 && Focus <= FOCUS_SCENE_4) {
                Sequencer.SetCurrentScene(Focus - FOCUS_SCENE_1);
            }
            break;

        case KEY_OK:
            if (Focus == FOCUS_TOP_MENU) {
                TopMenu.Event(event);
                return;
            }
            else if (Focus >= FOCUS_SCENE_1 && Focus <= FOCUS_SCENE_4){
                TSetupMenuPopup::Popup();
            }
            break;

        case KEY_BACK:
            if (Focus == FOCUS_TOP_MENU) {
                TopMenu.Event(event);
                return;
            }
            if (Focus == FOCUS_SCENE_MENU) {
                TSeqPage::LastSubpage--;
                return;
            }
            Sequencer.ToggleRunning();
            break;

        case KEY_LONGPRESS_BACK:
            Sequencer.Stop();
            Sequencer.Resync();
            break;

        case KNOB_RIGHT:
            Sequencer.ChangeNote(event_value(event), 1);
            break;

        case KNOB_LEFT:
            Sequencer.ChangeNote(event_value(event), -1);
            break;

        case KNOB_PUSH:
            if (Focus == FOCUS_SCENE_MENU) {
                Sequencer.ToggleSceneEnable(event_value(event));
            }
            else {
                Sequencer.ToggleEnable(event_value(event));
            }
            break;
        }
    }
}

void TSeqOverviewPage::Render(uint8_t n, TDisplay::TPageBuffer* line)
{
    uint8_t pos = LeftMargin;

    if (n == 1) {
        int pos = 0;
        int shadestart = 0;
        int shadeend = 0;

        for (int i = 0; i < 4; i++) {
            char str[5];
            str[0] = '0' + i;
            str[1] = Sequencer.Scenes[i].Flags & TSequencerScene::FLAG_ENABLED ? '\013' : '\014';
            str[2] = ' ';
            str[3] = '\0';
            pos = line->DrawText(str, pos);
        }

        shadestart = pos;
        pos = line->DrawText("\020", pos);
        pos = line->DrawText("all", pos, Focus == FOCUS_SCENE_MENU);
        pos = line->DrawText("\021", pos);
        shadeend = pos;

        line->Invert(0, shadestart);
        line->Invert(shadeend, line->GetLength());
    }
    else if (n >= 2 && n <= 5) {
        // Display a line with the scene's steps
        const int sceneno = n - 2;
        pos = 1;
        const TSequencerScene& scene = Sequencer.Scenes[sceneno];

        for (int step = 0; step < scene.Steps; step++) {
            if ((step % 4) == 0 && step != 0) {
                // A single pixel space between every fourth step
                line->Data[pos] = 0x7e;
                pos++;
            }
            if (scene.Data[step].Flags & TSequencerScene::TData::FLAG_ON) {
                // This step is on
                if (scene.Data[step].Action != 0) {
                    pos = line->DrawText("0", pos);
                } else {
                    pos = line->DrawText("o", pos);
                }
            }
            else {
                // This step is off
                if (scene.Data[step].Action != 0) {
                    pos = line->DrawText("=", pos);
                }
                else {
                    pos = line->DrawText("-", pos);
                }
            }

            if (Sequencer.GetPosition(sceneno).Step == step) {
                // This is the currently playing step for this scene
                if (scene.Flags & TSequencerScene::FLAG_ENABLED) {
                    line->Invert(pos - TDisplay::GlyphWidth, pos);
                }
                else {
                    line->Invert(pos - TDisplay::GlyphWidth, pos, 0x40);
                }
            }
        }
        if (Focus == FOCUS_SCENE_1 + sceneno) {
            line->Invert(0, line->GetLength());
        }
    }
    else if (n == 6) {
    }
    else if (n == 7) {
    	pos = line->DrawText("rst", pos, Focus == FOCUS_RESET);
    }
}

/*
 * ******************************************
 */

void TSetupMenuPopup::Popup()
{
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
            const uint8_t sceneno = Sequencer.GetCurrentScene();
            Sequencer.LoadFromMemory(sceneno, slot);
        }
        else if (selection == 8 && slot != -1) { // STORE...
            TNamePopup namePopup;

            char name[NAMELEN + 1];
            ::memset(name, 0, sizeof(name));
            uint32_t address = TMemory::BlockSize * (TMemory::BLOCK_FIRST_SEQ_SCENE + slot) + offsetof(TSequencerScene, Name);
            Memory.ReadBlocking(address, TBuffer(reinterpret_cast<uint8_t*>(name), NAMELEN));

            if (name[0]) {
                namePopup.SetName(name);
            } else {
                namePopup.SetName("no name");
            }

            if (namePopup.Show()) {
                const uint8_t sceneno = Sequencer.GetCurrentScene();
                Sequencer.StoreInMemory(sceneno, slot);
            }
        }
    }
}

void TSetupMenuPopup::GetMenuTitle(char text[MenuTextLen])
{
    cheap_strcpy(text, "Setup seq");
}

void TSetupMenuPopup::GetMenuItem(uint8_t n, char text[MenuTextLen])
{
    const uint8_t sceneno = Sequencer.GetCurrentScene();
    switch (n) {
    case 0:
        cheap_strcpy(text, "STEPS: xx");
        render_uint(text+7, Sequencer.Scenes[sceneno].Steps, 2);
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
        render_uint(text+10, Sequencer.Scenes[sceneno].Channel + 1, 2);
        break;
    case 4: {
        cheap_strcpy(text, "STEP: 1/xx   ");
        int step = Sequencer.GetStepLength(sceneno);
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
    const uint8_t sceneno = Sequencer.GetCurrentScene();
    switch (n) {
    case 0:
        Sequencer.ChangeSteps(value);
        break;
    case 1:
        Sequencer.ChangeTempo(value);
        break;
    case 3:
        Sequencer.Scenes[sceneno].Channel = clamp(Sequencer.Scenes[sceneno].Channel + value, 0, 15);
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



void TActionMenuPopup::Popup()
{
    int selection = -1;
    {
        TActionMenuPopup popup;
        selection = popup.Show();
        Gui.UpdateAll();
    }
    switch (selection) {
    case 0:
        break;
    }
}

void TActionMenuPopup::GetMenuTitle(char text[MenuTextLen])
{
    cheap_strcpy(text, "Action");
}

void TActionMenuPopup::GetMenuItem(uint8_t n, char text[MenuTextLen])
{
    switch (n) {
    case 0:
        cheap_strcpy(text, "Enable scene");
        break;
    }
}



TMemorySlotPopup::TMemorySlotPopup(const char* title) :
                        Title(title)
{
}

void TMemorySlotPopup::GetMenuTitle(char text[MenuTextLen])
{
    cheap_strcpy(text, Title);
}

void TMemorySlotPopup::GetMenuItem(uint8_t n, char text[MenuTextLen])
{
    if (n < TMemory::SEQ_SCENE_COUNT) {
        char name[NAMELEN + 1];
        ::memset(name, 0, sizeof(name));
        uint32_t address = TMemory::BlockSize * (TMemory::BLOCK_FIRST_SEQ_SCENE + n) + offsetof(TSequencerScene, Name);
        Memory.ReadBlocking(address, TBuffer(reinterpret_cast<uint8_t*>(name), NAMELEN));

        render_uint(text, n, 2);
        if (name[0]) {
            cheap_strncpy(text+2, name, MenuTextLen-1);
        }
        else {
            cheap_strcpy(text+2, "<empty>");
        }
    }
}
