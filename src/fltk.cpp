#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <FL/platform.H>	// fl_xid()

#include "fltk.h"

//
// fltk rendering window
//
// Make this as easy as possible on the main program
// so that we just provide a rendering surface, and hide
// all the details of widgets and such.
//

// Custom class to handle drawing
class MyApp : public Fl_Double_Window {
    // We use a callback to handle drawing
    DrawCallback *_draw_cb;

public:
    MyApp(int W,int H,const char*L=0) : Fl_Double_Window(W,H,L) {
        color(FL_BLACK);        // black window
        _draw_cb = 0;
	resizable(this);        // allow window to be resizable
    }

    void SetDrawCallback(DrawCallback *draw_cb) {
        _draw_cb = draw_cb;
    }

    void draw() {
        Fl_Double_Window::draw();       // let window draw itself (black)
        fl_color(FL_WHITE);             // draw with white lines
        fl_color(0x40ff4000);           // oops, no, green
        if ( _draw_cb ) _draw_cb();     // invoke caller's draw callback
        static int once = 0;
	if ( !once ) { once = 1; printf("\033[7mwindowid: %d\033[0m\n", get_win_id()); }
    }
};

// LOCAL GLOBALS
static MyApp *G_app = 0;

// init FLTK, set draw callback
void init_FLTK(DrawCallback *cb)
{
    G_app = new MyApp(FLTK_SCREEN_XMAX, FLTK_SCREEN_YMAX, "fltk");
    G_app->SetDrawCallback(cb);
    G_app->show();
}

void apploop_FLTK()
{
    Fl::run();
}

// Draw a line in FLTK window
//     Assumes MyApp::draw() already set the drawing color
//
void DrawLine_FLTK(int x1, int y1, int x2, int y2)
{
    fl_line(x1, y1, x2, y2);
}

// Current framerate
static float fltk_fps = 24.0;

void redraw_timer_cb(void*)
{
    G_app->redraw();
    Fl::repeat_timeout(1.0 / fltk_fps, redraw_timer_cb);
}

void Redraw_FLTK(float fps)
{
    fltk_fps = fps;
    Fl::add_timeout(1.0 / fltk_fps, redraw_timer_cb);
}
