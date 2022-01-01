// FLTK window rendering
#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Cairo_Window.H>

typedef void (DrawCallback)();

#define FLTKCAIRO_SCREEN_XMAX (ScreenWidth_FLTKCAIRO()-1)
#define FLTKCAIRO_SCREEN_YMAX (ScreenHeight_FLTKCAIRO()-1)

void Init_FLTKCAIRO(DrawCallback *cb);       // init FLTK/Cairo, set app draw callback
int  ScreenWidth_FLTKCAIRO();                // return current screen width
int  ScreenHeight_FLTKCAIRO();               // return current screen height
void AppLoop_FLTKCAIRO();                    // FLTK/Cairo app loop
void Redraw_FLTKCAIRO(float fps);            // trigger a redraws at a frame rate
void DrawLine_FLTKCAIRO(int x1, int y1, int x2, int y2);
