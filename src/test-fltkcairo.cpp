
// Test the 'fltkcairo.cpp' module
//
// This tests the abstraction of how drawing is handled in FLTK with Cairo.
// The goal of the fltkcairo.cpp module is to minimize FLTK's implementation details
// from the main program as much as possible.
//

#include "fltkcairo.h"

void draw_scene()
{
    // Draw an X
    DrawLine_FLTKCAIRO(0, 0,                     FLTKCAIRO_SCREEN_XMAX, FLTKCAIRO_SCREEN_YMAX);
    DrawLine_FLTKCAIRO(0, FLTKCAIRO_SCREEN_YMAX, FLTKCAIRO_SCREEN_XMAX, 0);
}

int main()
{
    Init_FLTKCAIRO(draw_scene);
    AppLoop_FLTKCAIRO();
}
