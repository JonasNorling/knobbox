/* -*- c++ -*- */
#pragma once

#include <cstdint>
#include "TDisplayPageBase.h"
#include "TGui.h"
#include "TPopup.h"

/**
 * The user interface page that is displayed when the device is in
 * MIDI controller mode.
 *
 * Status: Mostly a mockup. Moving around works, some knob updates
 * work.
 */
class TControllerPage : public TDisplayPageBase
{
public:
    enum TFocus { FOCUS_TOP_MENU, FOCUS_CHANNEL, FOCUS_SCENE,
        FOCUS_LAST = FOCUS_SCENE };

    TControllerPage() : CurrentChannel(7), Focus(FOCUS_TOP_MENU) { }

    void Show();
    void Render(uint8_t n, TDisplay::TPageBuffer* line);

private:
    TTopMenu TopMenu;

    uint8_t CurrentChannel; ///< This shouldn't be here

    uint8_t Focus;
};

class TChannelSelectPopup : public TSelectPopup
{
    //int Show(int selection); uses default from TSelectPopup
    virtual void GetMenuTitle(char text[MenuTextLen]);
    virtual void GetMenuItem(uint8_t n, char text[MenuTextLen]);
};
