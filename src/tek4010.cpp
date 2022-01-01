// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

// tek40xx screen is:
//
//  0,779  __________________________  1023,779
//        |                          |
//        |                          |
//        |                          |
//        |                          |
//        |                          |
//    0,0 |__________________________| 1023,0
//

#include <stdio.h>
#include <math.h>
#include "tek4010.h"

#define MAXX TEK4010_SCREEN_XMAX	// 1024
#define MAXY TEK4010_SCREEN_YMAX	// 780

static int xs,ys;

static void startDraw(int x1,int y1)
{
    if (x1 < 0) x1 = 0;
    if (x1 >= MAXX) x1 = MAXX -1;
    if (y1 < 0) y1 = 0;
    if (y1 >= MAXY) y1 = MAXY - 1;
    putchar(29);
    putchar((y1 >> 5) + 32);
    putchar((y1 & 31) + 96);
    putchar((x1 >> 5) + 32);
    putchar((x1 & 31) + 64);
    xs = x1;
    ys = y1;
}

static void draw(int x2,int y2)
{
    int hxchange, lychange;

    if (x2 < 0) x2 = 0;
    if (x2 >= MAXX) x2 = MAXX -1;
    if (y2 < 0) y2 = 0;
    if (y2 >= MAXY) y2 = MAXY - 1;

    if ((y2 >> 5) != (ys >> 5))        /* if high y has changed */
	putchar((y2 >> 5) + 32);
    hxchange = (x2 >> 5) != (xs >> 5);
    lychange = (y2 & 31) != (ys & 31);
    if (hxchange || lychange) putchar((y2 & 31) + 96);
    if (hxchange)                                     /* if high order x has changed */
    	putchar((x2 >> 5) + 32);
    putchar((x2 & 31) + 64);
    xs = x2;
    ys = y2;
}

static void endDraw()
{
    putchar(31);
    fflush(stdout);
}

// Enter graphing mode.
void init_TEK4010()
{
    printf("\035");	// GS
}

// Erases screen.
//     Resets X=0 (left), Y=767(top).
//     Also resets GIN, Echoplex Suppression, Margin, and Graph.
//
void Clear_TEK4010()
{
    putchar(27);	// 033 (ESC)
    putchar(12);	// 014 (FF)
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Coehn-Sutherland line clipping
// Nabbed from: https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm#Example_C/C++_implementation
//
typedef int OutCode;
static const int INSIDE = 0; // 0000
static const int LEFT   = 1; // 0001
static const int RIGHT  = 2; // 0010
static const int BOTTOM = 4; // 0100
static const int TOP    = 8; // 1000
static const int xmin   = 0;
static const int ymin   = 0;
static const int xmax   = TEK4010_SCREEN_XMAX;
static const int ymax   = TEK4010_SCREEN_YMAX;

// Compute the bit code for a point (x, y) using the clip
// bounded diagonally by (xmin, ymin), and (xmax, ymax)
// ASSUME THAT xmax, xmin, ymax and ymin are global constants.
static OutCode ComputeOutCode(float x, float y)
{
    OutCode code;
    code = INSIDE;          // initialised as being inside of [[clip window]]
    if (x < xmin)           // to the left of clip window
        code |= LEFT;
    else if (x > xmax)      // to the right of clip window
        code |= RIGHT;
    if (y < ymin)           // below the clip window
        code |= BOTTOM;
    else if (y > ymax)      // above the clip window
        code |= TOP;
    return code;
}

// Cohen–Sutherland clipping algorithm clips a line from
// P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with
// diagonal from (xmin, ymin) to (xmax, ymax).
//
// ERCO: Returns false if line should not be drawn at all.
//       Returns true if line should be drawn with x0,y0,x1,y1 clipped as necessary
//
static bool CohenSutherlandLineClipAndDraw(int &in_x0, int &in_y0, int &in_x1, int &in_y1)
{
    // Convert to floats
    float x0 = in_x0;
    float y0 = in_y0;
    float x1 = in_x1;
    float y1 = in_y1;

    // compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
    OutCode outcode0 = ComputeOutCode(x0, y0);
    OutCode outcode1 = ComputeOutCode(x1, y1);
    bool accept = false;

    while (true) {
        if (!(outcode0 | outcode1)) {
            // bitwise OR is 0: both points inside window; trivially accept and exit loop
            accept = true;
            break;
        } else if (outcode0 & outcode1) {
            // bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
            // or BOTTOM), so both must be outside window; exit loop (accept is false)
            //
            break;
        } else {
            // failed both tests, so calculate the line segment to clip
            // from an outside point to an intersection with clip edge
            //
            float x, y;

            // At least one endpoint is outside the clip rectangle; pick it.
            OutCode outcodeOut = outcode1 > outcode0 ? outcode1 : outcode0;

            // Now find the intersection point;
            // use formulas:
            //   slope = (y1 - y0) / (x1 - x0)
            //   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
            //   y = y0 + slope * (xm - x0), where xm is xmin or xmax
            // No need to worry about divide-by-zero because, in each case, the
            // outcode bit being tested guarantees the denominator is non-zero
            //
            if (outcodeOut & TOP) {           // point is above the clip window
                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                y = ymax;
            } else if (outcodeOut & BOTTOM) { // point is below the clip window
                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            } else if (outcodeOut & RIGHT) {  // point is to the right of clip window
                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            } else if (outcodeOut & LEFT) {   // point is to the left of clip window
                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }

            // Now we move outside point to intersection point to clip
            // and get ready for next pass.
            if (outcodeOut == outcode0) {
                x0 = x;
                y0 = y;
                outcode0 = ComputeOutCode(x0, y0);
            } else {
                x1 = x;
                y1 = y;
                outcode1 = ComputeOutCode(x1, y1);
            }
        }
    }

    // Convert clipped results back to ints
    in_x0 = int(x0 + 0.5); // + 0.5 assumes all values positive
    in_y0 = int(y0 + 0.5);
    in_x1 = int(x1 + 0.5);
    in_y1 = int(y1 + 0.5);

    return accept;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Draw a line between two points at current intensity
void DrawLine_TEK4010(int x1, int y1, int x2, int y2)
{
    // if ( x1<0    && x2<0    ) return;
    // if ( x1>MAXX && x2>MAXX ) return;
    // if ( y1<0    && y2<0    ) return;
    // if ( y1>MAXY && y2>MAXY ) return;

    // Handle clipping to the xmin/xmax/ymin/ymax of window.
    //    If completely clipped, don't draw at all
    //
    if ( CohenSutherlandLineClipAndDraw(x1,y1,x2,y2) == false ) return;
    startDraw(x1,y1); draw(x2,y2); // endDraw();
}

#if 0
int main (int argc, char *argv[])
{
    // DRAW AN X ON TEKTRONIX 4010
    printf("\033\014");		// clear screen
    startDraw(     0,0); draw(MAXX-1, MAXY-1); endDraw();
    startDraw(MAXX-1,0); draw(     0, MAXY-1); endDraw();
    startDraw(1,1); endDraw(); // move the cursor out of the way
}
#endif

#if 0
#define xmax 1024
#define ymax  780


// Return to text mode
void TEK4010_TextMode()
{
    printf("\n");
}

static void startplotpoint()
{
    putchar(27);	// 033 (ESC)
    putchar(28);	// 034 (FS)
}

static void stopplotpoint()
{
    printf("\n");
}

// See TABLE 2-4 on pp 2-10 (PDF Page 30) of tektronix-4012-users-manual
//   1. Sending GS enters graphics mode
//   2. X/Y positioning 0-1023 can be addressed as follows:
//      2a. Convert Y to 10 binary digits (Y = Y & 0x03ff)
//          Convert X to 10 binary digits (X = X & 0x03ff)
//      2b. Form a Hi Y byte by affixing 01 (as bits 7 and 6) to the 5 MSB of the ten digits
//          of the Y coordinate.
//      2c. Form a Lo Y byte by affixing 11 (as bits 7 and 6) to the 5 LSB of the ten digits
//          of the Y coordinate.
//      2d. Form a Hi X byte by affixing 01 (as bits 7 and 6) to the 5 MSB of the ten digits
//          of the X coordinate.
//      2e. Form a Lo X byte by affixing 10 (as bits 7 and 6) to the 5 LSB of the ten digits
//          of the X coordinate.
//      2f. Send the four bytes as formed in (2b) through (2e).
//
//   Uh, if you can understand what any of that means, congradulations.
//
//             76543210
//       HiY:  010YYYYY
//             --_-----
//             | |  |____ 5 MSB of Y
//             | |_______ zero? I guess?
//             |_________ "01 as bits 7 and 6", e.g. 01000000b == 64d == 0x40
//
//       LoY:  110YYYYY
//             --_-----
//             | |  |____ 5 LSB of Y
//             | |_______ zero? I guess?
//             |_________ "11 as bits 7 and 6", e.g. 11000000b == 192d == 0xc0
//
//       
static void draw(int x2,int y2)
{
    int hxchange, lychange;

    if ((y2 >> 5) != (ys >> 5)) {       /* if high y has changed */
	putchar((y2 >> 5) + 32);
	count++;
    }
    hxchange = (x2 >> 5) != (xs >> 5);
    lychange = (y2 & 31) != (ys & 31);
    if (hxchange || lychange) {
	putchar((y2 & 31) + 96);
	count++;
    }
    if (hxchange) {                                    /* if high order x has changed */
    	putchar((x2 >> 5) + 32);
	count++;
    }
    putchar((x2 & 31) + 64);
    count++;
    xs = x2;
    ys = y2;
}

static void plotpoint(int x,int y,double intensity)
{
    int intensity1;
    if (intensity == 0) intensity1 = 64;
    else if (intensity <= 1) intensity1 = 70;
    else if (intensity <= 2) intensity1 = 75;
    else if (intensity <= 3) intensity1 = 79;
    else if (intensity <= 4) intensity1 = 82;
    else if (intensity <= 5) intensity1 = 85;
    else if (intensity <= 6) intensity1 = 87;
    else if (intensity <= 7) intensity1 = 88;
    else if (intensity <= 8) intensity1 = 89;
    else if (intensity <= 9) intensity1 = 90;
    else if (intensity <= 10) intensity1 = 91;
    else if (intensity <= 11) intensity1 = 92;
    else if (intensity <= 12) intensity1 = 94;
    else if (intensity <= 13) intensity1 = 95;
    else if (intensity <= 14) intensity1 = 96;
    else if (intensity <= 16) intensity1 = 97;
    else if (intensity <= 17) intensity1 = 98;
    else if (intensity <= 19) intensity1 = 99;
    else if (intensity <= 20) intensity1 = 100;
    else if (intensity <= 22) intensity1 = 101;
    else if (intensity <= 23) intensity1 = 102;
    else if (intensity <= 25) intensity1 = 103;
    else if (intensity <= 28) intensity1 = 104;
    else if (intensity <= 31) intensity1 = 105;
    else if (intensity <= 34) intensity1 = 106;
    else if (intensity <= 38) intensity1 = 107;
    else if (intensity <= 41) intensity1 = 108;
    else if (intensity <= 44) intensity1 = 109;
    else if (intensity <= 47) intensity1 = 110;
    else if (intensity <= 50) intensity1 = 111;
    else if (intensity <= 56) intensity1 = 112;
    else if (intensity <= 62) intensity1 = 113;
    else if (intensity <= 69) intensity1 = 114;
    else if (intensity <= 75) intensity1 = 115;
    else if (intensity <= 81) intensity1 = 116;
    else if (intensity <= 88) intensity1 = 117;
    else if (intensity <= 94) intensity1 = 118;
    else intensity1 = 119;
    putchar(intensity1); count++;
    draw(x,y);
}
#endif
