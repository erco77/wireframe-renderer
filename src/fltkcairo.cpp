#include <FL/Fl.H>
#include <FL/Fl_Cairo_Window.H>	// fltk's cairo window
#include <FL/fl_draw.H>
#include "fltkcairo.h"

//
// fltk/cairo rendering window - leverage cairo's antialiased line drawing
//
// Make this as easy as possible on the main program
// so that we just provide a rendering surface, and hide
// all the details of widgets and such.
//

#define DEFAULT_WIDTH  1000
#define DEFAULT_HEIGHT 1000

// Custom class to handle drawing
class MyApp : public Fl_Cairo_Window {
    // User's callback to handle drawing, but prevent user app
    // from having to know anything about cairo.
    // 
    DrawCallback *user_draw_cb;
    cairo_t *cr;  // set by fltkcairo_draw_cb(), used by DrawLine_FLTKCAIRO()
public:

    static void fltkcairo_draw_cb(Fl_Cairo_Window *w, cairo_t *cr) {
        extern MyApp *G_app;
        G_app->cr = cr;                                    // Save cr for DrawLine_FLTKCAIRO() to use
	cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);     // use best antialiasing
	cairo_set_line_width(cr, 0.20);                    // line width for drawing
	cairo_set_source_rgb(cr, 0.3, 1.0, 0.3);           // bright green
	if ( G_app->user_draw_cb ) G_app->user_draw_cb();
    }
public:
    MyApp(int W,int H,const char*L=0) : Fl_Cairo_Window(W,H,L) {
        color(FL_BLACK);        // black window (allows for long camera exposures of vectors)
        user_draw_cb = 0;
	size_range(50,50,-1,-1);
	resizable(this);        // allow window to be resizable
	set_draw_cb(fltkcairo_draw_cb);   // Fl_Cairo_Window's draw callback
    }

    void SetDrawCallback(DrawCallback *cb) {
        user_draw_cb = cb;
    }
    friend void fltkcairo_draw_cb(Fl_Cairo_Window *w, cairo_t *cr);
    friend void DrawLine_FLTKCAIRO(int,int,int,int);
};

// LOCAL GLOBALS
MyApp *G_app = 0;

// init FLTK, set draw callback
void Init_FLTKCAIRO(DrawCallback *cb)
{
    // Create custom cairo window, setup app's draw callback
    G_app = new MyApp(DEFAULT_WIDTH, DEFAULT_HEIGHT, "fltkcairo");
    G_app->SetDrawCallback(cb);
    G_app->show();
}

// Return screen size to app
int ScreenWidth_FLTKCAIRO()  { return G_app ? G_app->w() : DEFAULT_WIDTH; }
int ScreenHeight_FLTKCAIRO() { return G_app ? G_app->h() : DEFAULT_HEIGHT; }

void AppLoop_FLTKCAIRO()
{
    Fl::run();
}

// Current framerate
static float fltkcairo_fps = 24.0;

static void redraw_timer_cb(void*)
{
    G_app->redraw();
    Fl::repeat_timeout(1.0 / fltkcairo_fps, redraw_timer_cb);
}

void Redraw_FLTKCAIRO(float fps)
{
    fltkcairo_fps = fps;
    Fl::add_timeout(1.0 / fltkcairo_fps, redraw_timer_cb);
}

// Draw a line in Fl_Cairo_Window
//     Assumes MyApp::fltkcairo_draw_cb() already set up
//     drawing color, line width, AA line mode, etc.
//
void DrawLine_FLTKCAIRO(int x1, int y1, int x2, int y2)
{
    cairo_move_to(G_app->cr, x1, y1);
    cairo_line_to(G_app->cr, x2, y2);
    cairo_stroke(G_app->cr);
}
