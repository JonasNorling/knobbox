/* -*- c++ -*- */
#pragma once

class TPopup
{
public:
  TPopup() :
    Scroll(0),
    Focus(0),
    ItemCount(255)
  {}
  virtual void Render(uint8_t n, TDisplay::TPageBuffer* line);
  virtual void Event(TEvent /*event*/) {}

protected:
  static const uint8_t Margin = 13;
  static const uint8_t Lines = 7;
  uint8_t Scroll;
  uint8_t Focus;
  uint8_t ItemCount;
};

/**
 * Present a popup menu that allows tweaking settings by moving to an
 * item, pressing OK, and making adjustments with up/down. The popup
 * is left by the back button.
 */
class TSettingsPopup : public TPopup
{
public:
  virtual void Event(TEvent event);

private:
  bool Selected;
};

/**
 * Present a popup menu that returns when an item is selected by
 * moving the cursor with up/down buttons and pressing OK.
 */
class TSelectPopup : public TPopup
{
public:
  virtual void Event(TEvent event);
};

class TButtonPopup : public TPopup
{
};
