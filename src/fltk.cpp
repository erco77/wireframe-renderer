#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <FL/platform.H>	// fl_xid()
#include <string>		// std::string
#include <stdlib.h>		// system()

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
    std::string _post_render_cmd;

public:
    MyApp(int W,int H,const char*L=0) : Fl_Double_Window(W,H,L) {
        color(FL_BLACK);        // black window
        _draw_cb = 0;
	resizable(this);        // allow window to be resizable
    }

    void SetDrawCallback(DrawCallback *draw_cb) {
        _draw_cb = draw_cb;
    }

    void SetPostRenderCommand(const char *cmd) {
        _post_render_cmd = cmd;
    }

    void draw() {
        Fl_Double_Window::draw();       // let window draw itself (black)
        fl_color(FL_WHITE);             // draw with white lines
        fl_color(0x40ff4000);           // oops, no, green
	//DEBUG printf("fltk::draw() called, draw_cb is %p\n", _draw_cb);
        if ( _draw_cb ) {     		// invoke caller's draw callback
	    _draw_cb();
            static int frame = 0;
	    frame++;
	    if ( _post_render_cmd != "" ) {
	        static char winid_env[40];
	        static char frame_env[40];
//XXX:	        sprintf(winid_env, "FLTK_WIN_ID=%d", get_win_id()); putenv(winid_env); // can be used by 'screencapture -l'
	        sprintf(frame_env, "FRAME=%04d", frame);            putenv(frame_env); // can be used by external script
	        printf("\n");
	        printf("Environment variable: %s\n", winid_env);
	        printf("Environment variable: %s\n", frame_env);
	        printf("\033[7m--- EXECUTING: %s\033[0m\n", _post_render_cmd.c_str());
	        system(_post_render_cmd.c_str());
	    }
	}
    }
};

// LOCAL GLOBALS
static MyApp *G_app = 0;

// init FLTK, set draw callback
void Init_FLTK(DrawCallback *cb)
{
    G_app = new MyApp(FLTK_DEFAULT_XMAX, FLTK_DEFAULT_YMAX, "fltk");
    G_app->SetDrawCallback(cb);
    G_app->show();
    //XXX G_app->wait_for_expose();          // make sure window is mapped
    //XXX Fl::flush();		// flush FLTK graphics before calling render cmd
}

void AppLoop_FLTK()
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

void SetPostRenderCommand_FLTK(const char *cmd)
{
    if ( G_app ) G_app->SetPostRenderCommand(cmd);
}

int GetScreenWidth_FLTK()  { return G_app ? G_app->w() : FLTK_DEFAULT_XMAX; }
int GetScreenHeight_FLTK() { return G_app ? G_app->h() : FLTK_DEFAULT_YMAX; }
