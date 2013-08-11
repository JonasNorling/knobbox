#include "device.h"
#include <cstdio>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <wx/wx.h>

class WxApp: public wxApp
{
public:
    virtual bool OnInit();
};

class WxFrame: public wxFrame
{
public:
    WxFrame();
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};

WxFrame::WxFrame() :
        wxFrame(NULL, -1, _("Knobbox"))
{
    wxPanel* panel = new wxPanel(this, -1);
    wxButton* b = new wxButton(panel, -1, _("1"), wxPoint(100, 100), wxSize(30, 30));
}

bool WxApp::OnInit()
{
    wxFrame* frame = new WxFrame;
    frame->Show();
    SetTopWindow(frame);
    return true;
}

static void* gui_thread_main(void*)
{
    int argc = 1;
    char* argv[] = { "name" };

    wxApp* app = new WxApp;
    wxApp::SetInstance(app);

    ::wxEntry(argc, argv);

    return NULL;
}

static void alarm_handler(int)
{
    sys_tick_handler();
}

void deviceInit()
{
    // Spawn a thread and start a wxWidgets instance
    pthread_t guithread;
    pthread_create(&guithread, NULL, gui_thread_main, NULL);

    struct sigaction sa;
    sa.sa_handler = alarm_handler;
    sigaction(SIGALRM, &sa, NULL);
    timer_t timerid;
    timer_create(CLOCK_MONOTONIC, NULL, &timerid);
    struct itimerspec spec = { { 0, 1000000 }, { 1, 0 } };
    timer_settime(timerid, 0, &spec, NULL);
}

void usart_send(uint32_t uart, uint8_t byte)
{
    printf("UART%d: 0x%02x\n", uart, byte);
}
