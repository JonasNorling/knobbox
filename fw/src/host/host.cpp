#include "device.h"
#include <host/WxLcdDisplay.h>
#include <cstdio>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <wx/wx.h>
#include <TDisplay.h>

WxLcdDisplay* LcdDisplay;

class WxApp: public wxApp
{
public:
    virtual bool OnInit();
};

enum {
    ID_BUTTON_SELECT = 1,
    ID_BUTTON_KNOB_CCW,
    ID_BUTTON_KNOW_CW
};

class WxFrame: public wxFrame
{
public:
    WxFrame();

private:
    void OnButtonClick(wxCommandEvent& event);

    DECLARE_EVENT_TABLE();
};

BEGIN_EVENT_TABLE(WxFrame, wxFrame)
    EVT_BUTTON(ID_BUTTON_SELECT, WxFrame::OnButtonClick)
    EVT_BUTTON(ID_BUTTON_KNOB_CCW, WxFrame::OnButtonClick)
    EVT_BUTTON(ID_BUTTON_KNOW_CW, WxFrame::OnButtonClick)
END_EVENT_TABLE()

WxFrame::WxFrame() :
        wxFrame(NULL, -1, _("Knobbox"))
{
    LcdDisplay = new WxLcdDisplay(this);

    wxBoxSizer* vsizer = new wxBoxSizer(wxVERTICAL);
    vsizer->Add(LcdDisplay, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

    wxBoxSizer* hsizer = new wxBoxSizer(wxHORIZONTAL);
    vsizer->Add(hsizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
    hsizer->Add(new wxButton(this, ID_BUTTON_KNOB_CCW, _("^<-")));
    hsizer->Add(new wxButton(this, ID_BUTTON_SELECT, _("select")));
    hsizer->Add(new wxButton(this, ID_BUTTON_KNOW_CW, _("+>v")));

    hsizer = new wxBoxSizer(wxHORIZONTAL);
    vsizer->Add(hsizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
    hsizer->Add(new wxButton(this, -1, _("<")));
    hsizer->Add(new wxButton(this, -1, _("<<")));
    hsizer->Add(new wxButton(this, -1, _("P")));
    hsizer->Add(new wxButton(this, -1, _("-")));
    hsizer->Add(new wxButton(this, -1, _(">")));

    SetSizerAndFit(vsizer);
    SetAutoLayout(true);
}

void WxFrame::OnButtonClick(wxCommandEvent& event)
{
    printf("Click %d\n", event.GetId());

}


bool WxApp::OnInit()
{
    wxFrame* frame = new WxFrame;
    frame->Show();
    SetTopWindow(frame);
    return true;
}

void guiStart(void)
{
    int argc = 1;
    char* argv[] = { "name" };

    wxApp* app = new WxApp;
    wxApp::SetInstance(app);

    ::wxEntry(argc, argv);
}

static void alarm_handler(int)
{
    sys_tick_handler();
}

void deviceInit()
{
}

void usart_send(uint32_t uart, uint8_t byte)
{
    printf("UART%d: 0x%02x\n", uart, byte);
}
