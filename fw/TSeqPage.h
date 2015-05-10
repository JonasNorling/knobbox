/* -*- c++ -*- */
#pragma once

#include <cstdint>
#include "TDisplayPageBase.h"
#include "TGui.h"
#include "TPopup.h"

class TSeqPage : public TDisplayPageBase
{
public:
    enum TFocus {
        // Currently focused field
        FOCUS_TOP_MENU, FOCUS_SCENE_MENU, FOCUS_SETUPMENU,
        FOCUS_STEP, FOCUS_NOTE, FOCUS_ACTION,
        FOCUS_VELO, FOCUS_LEN, FOCUS_OFFSET, FOCUS_CC,
        FOCUS_TEMPO,
        FOCUS_LAST = FOCUS_TEMPO
    };

    TSeqPage(TFocus focus = FOCUS_TOP_MENU) : Focus(focus), CurrentStep(0),
            Selected(false), Blink(false) { }
    void Show();
    void Render(uint8_t n, TDisplay::TPageBuffer* line);
    static uint8_t LastSubpageWasOverview() { return LastSubpage == 4; }

private:
    friend class TSeqOverviewPage;

    TTopMenu TopMenu;

    uint8_t Focus;
    uint8_t CurrentStep;
    bool Selected; /// Focused item is selected and blinking
    bool Blink;
    static uint8_t LastSubpage;

    // Event handlers for the current focus, should return true if the
    // event was consumed.
    typedef bool (TSeqPage::*eventhandler_t) (TEvent);
    const static eventhandler_t EventHandler[FOCUS_LAST + 1];

    bool EventHandlerScene(TEvent event);
    bool EventHandlerSetup(TEvent event);
    bool EventHandlerStep(TEvent event);
    bool EventHandlerNote(TEvent event);
    bool EventHandlerAction(TEvent event);
    bool EventHandlerVelo(TEvent event);
    bool EventHandlerLen(TEvent event);
    bool EventHandlerOffset(TEvent event);
    bool EventHandlerTempo(TEvent event);
};

class TSeqOverviewPage : public TDisplayPageBase
{
public:
    enum TFocus {
        // Currently focused field
        FOCUS_TOP_MENU,
        FOCUS_SCENE_MENU,
        FOCUS_SCENE_1,
        FOCUS_SCENE_2,
        FOCUS_SCENE_3,
        FOCUS_SCENE_4,
        FOCUS_RESET,
        FOCUS_LAST = FOCUS_RESET
    };

    TSeqOverviewPage(TFocus focus = FOCUS_TOP_MENU) : Focus(focus) { }
    void Show();
    void Render(uint8_t n, TDisplay::TPageBuffer* line);

private:
    TTopMenu TopMenu;

    uint8_t Focus;
};

class TSetupMenuPopup : public TSettingsPopup
{
public:
    static void Popup();
    virtual void ItemChanged(uint8_t n, int8_t value);
    virtual bool ItemSelected(uint8_t n);
    virtual void GetMenuTitle(char text[MenuTextLen]);
    virtual void GetMenuItem(uint8_t n, char text[MenuTextLen]);
};

class TActionMenuPopup : public TSelectPopup
{
public:
    static void Popup();
    virtual void GetMenuTitle(char text[MenuTextLen]);
    virtual void GetMenuItem(uint8_t n, char text[MenuTextLen]);
};

class TMemorySlotPopup : public TSelectPopup
{
public:
    TMemorySlotPopup(const char* title);
    virtual void GetMenuTitle(char text[MenuTextLen]);
    virtual void GetMenuItem(uint8_t n, char text[MenuTextLen]);

private:
    const char* Title;
};
