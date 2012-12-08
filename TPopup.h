/* -*- c++ -*- */
#pragma once

#include "TMemory.h"
#include "IDisplayPage.h"

class TPopup
{
public:
    static const uint8_t MenuTextLen = TDisplayPageBase::MenuTextLen;

    TPopup() :
        Scroll(0),
        Focus(0),
        ItemCount(255)
    {}

    void Render(uint8_t n, TDisplay::TPageBuffer* line);

    virtual void GetMenuTitle(char text[MenuTextLen]) = 0;
    virtual void GetMenuItem(uint8_t n, char text[MenuTextLen]) = 0;

protected:
    static const uint8_t Margin = 13;
    static const uint8_t Lines = 7;
    uint8_t Scroll;
    uint8_t Focus;
    uint8_t ItemCount;
    bool Selected;
};

/**
 * Present a popup menu that allows tweaking settings by moving to an
 * item, pressing OK, and making adjustments with up/down. The popup
 * is left by the back button.
 */
class TSettingsPopup : public TPopup
{
public:
    TSettingsPopup() {}
    /** Show menu blockingly. Return selected item or -1. */
    int Show();
    /** Callback on item change (value is +1 or -1) */
    virtual void ItemChanged(uint8_t n, int8_t value) = 0;
    /** Callback on item select. Return true to cause Show() to return the item. */
    virtual bool ItemSelected(uint8_t n) = 0;
};

/**
 * Present a popup menu that returns when an item is selected by
 * moving the cursor with up/down buttons and pressing OK.
 */
class TSelectPopup : public TPopup
{
public:
    TSelectPopup() {}
    int Show(int initFocus = 0);
};

/**
 * Present a popup menu that allows inputting a name.
 */
class TNamePopup
{
public:
    TNamePopup() : Focus(0), ItemCount(0) {}
    void SetName(const char*);
    void Render(uint8_t n, TDisplay::TPageBuffer* line);
    bool Show();

private:
    static const uint8_t Margin = 13;
    static const uint8_t MenuTextLen = TPopup::MenuTextLen;

    char Name[NAMELEN];
    uint8_t Focus;
    uint8_t ItemCount;
};

class TButtonPopup : public TPopup
{
};
