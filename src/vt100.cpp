// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#include <stdio.h>
#include "vt100.h"

void init_VT100()
{
    // nothing to do
}

// local draw function that uses VT100 ANSI sequences
static void DrawFunc(int x, int y)
{
    // Screen clipping
    if ( (x < 0) || (x >= VT100_SCREEN_XMAX) ) return;
    if ( (y < 0) || (y >= VT100_SCREEN_YMAX) ) return;
    printf("\033[%d;%dH#", y, x);
}

// Bresenham to draw between two points
#define ABS(x) ((x)<0?-(x):(x))
static void DrawLine(int x1, int y1, 
                     int x2, int y2,
                     int step,
                     void (*drawfunc)(int,int)) // function to output x,y values
{
    // Plot a single point?
    if ( x1 == x2 && y1 == y2 ) {
	(*drawfunc)(x1, y1);
	return;
    }

    long x,y,xInc,yInc;
    long dx,dy;
    int swap;
    int skip = 0;

    //DEBUG fprintf(stderr, "Draw %d,%d -> %d,%d step %d (dx=%ld dy=%ld)\n", x1,y1, x2,y2, step, dx, dy);

    dx = ( x2 - x1 );
    dy = ( y2 - y1 );

    if ( ABS( dx ) > ABS( dy ) ) {
        if ( dx < 0 ) {
             dx = -dx;
             dy = -dy;
             swap = y2;
             y2 = y1;
             y1 = swap;
             swap = x2;
             x2 = x1;
             x1 = swap;
        }
        x = x1 << 16;
        y = y1 << 16;
        if ( dx == 0 ) dx = 1;
        yInc = ( dy * 65536 ) / dx;
             
        while ( ( x >> 16 ) <= x2 ) {
            if ( ( skip++ % step ) == 0 )
                (*drawfunc)( x >> 16,y >> 16);
            x += ( 1 << 16 );
            y += yInc;
        }
    } else {
        if ( dy < 0 ) {
            dx = -dx;
            dy = -dy;
            swap = y2;
            y2 = y1;
            y1 = swap;
            swap = x2;
            x2 = x1;
            x1 = swap;
        }
        x = x1 << 16;
        y = y1 << 16;
        if ( dy==0 ) dy = 1;
        xInc = ( dx * 65536 ) / dy;
        while ( ( y >> 16 ) <= y2 ) {
            if ( ( skip++ % step ) == 0 )
                (*drawfunc)( x >> 16,y >> 16);
            x += xInc;
            y += ( 1 << 16 );
        }
    }
}

void Clear_VT100()
{
    printf("\033[2J\r");
    fflush(stdout);
}

void DrawLine_VT100(int x1, int y1, int x2, int y2)
{
    const int step = 1;
    DrawLine(x1,y1,x2,y2,step,DrawFunc);
}
