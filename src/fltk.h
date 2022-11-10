// FLTK window rendering
#pragma once

#define FLTK_DEFAULT_XMAX 800
#define FLTK_DEFAULT_YMAX 800
#define FLTK_SCREEN_XMAX GetScreenWidth_FLTK()
#define FLTK_SCREEN_YMAX GetScreenHeight_FLTK()

typedef void (DrawCallback)(void);

void Init_FLTK(DrawCallback *drawcb);             // init FLTK, set draw callback
void AppLoop_FLTK();                              // init FLTK, set draw callback
void DrawLine_FLTK(int x1,int y1,int x2,int y2);  // Draw a line in FLTK window
void Redraw_FLTK(float fps);                      // trigger a redraw
void SetPostRenderCommand_FLTK(const char *cmd);  // sets a post render command
int  GetScreenWidth_FLTK();
int  GetScreenHeight_FLTK();
