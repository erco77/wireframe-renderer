// Test the 'fltk.cpp' module
//
// This tests the abstraction of how drawing is handled in FLTK.
// The goal of the fltk.cpp module is to minimize FLTK's implementation details
// from the main program as much as possible.
//

#include "fltk.h"

void draw_scene()
{
    // Draw an X
    DrawLine_FLTK(0, 0,                FLTK_SCREEN_XMAX, FLTK_SCREEN_YMAX);
    DrawLine_FLTK(0, FLTK_SCREEN_YMAX, FLTK_SCREEN_XMAX, 0);
}

int main()
{
    Init_FLTK(draw_scene);
    AppLoop_FLTK();
}
