// FLTK window rendering
#pragma once

#define FLTK_SCREEN_XMAX 800
#define FLTK_SCREEN_YMAX 800

typedef void (DrawCallback)(void);

void init_FLTK(DrawCallback *drawcb);             // init FLTK, set draw callback
void apploop_FLTK();                              // init FLTK, set draw callback
void DrawLine_FLTK(int x1,int y1,int x2,int y2);  // Draw a line in FLTK window
void Redraw_FLTK(float fps);                      // trigger a redraw
