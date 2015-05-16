#pragma once

#include <wx/wx.h>

class WxLcdDisplay : public wxPanel
{
public:
    WxLcdDisplay(wxFrame* parent);

    static const int Width = 102;
    static const int Height = 64;
    bool Framebuffer[Width][Height];

    void paintEvent(wxPaintEvent & evt);
    void paintNow();

    void render(wxDC& dc);

    DECLARE_EVENT_TABLE()
};
