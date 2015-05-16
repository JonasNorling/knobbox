#include "WxLcdDisplay.h"

BEGIN_EVENT_TABLE(WxLcdDisplay, wxPanel)
EVT_PAINT(WxLcdDisplay::paintEvent)
END_EVENT_TABLE()

WxLcdDisplay::WxLcdDisplay(wxFrame* parent)
    : wxPanel(parent, -1, wxDefaultPosition, wxSize(Width*3, Height*3))
{
    ::memset(Framebuffer, 0, sizeof(Framebuffer));
}

void WxLcdDisplay::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void WxLcdDisplay::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void WxLcdDisplay::render(wxDC& dc)
{
    dc.SetBackground(*wxLIGHT_GREY_BRUSH);
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.Clear();

    for (int x = 0; x < Width; x++) {
        for (int y = 0; y < Height; y++) {
            if (Framebuffer[x][y]) {
                dc.DrawRectangle(3*x, 3*y, 3, 3);
            }
        }
    }
}
