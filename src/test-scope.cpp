// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "scope.h"

//
// Oscilloscope/WAV File Rendering - Renders vectors to .wav file
//                                   Note: the screen space is HUGE; full signed 16bit numbers.
//
const int   SCREEN_WIDTH  = 65535;   // Lets use unsigned 0 .. 65535
const int   SCREEN_HEIGHT = 65535;   // since we're used to this.
const float SCREEN_XSCALE = 1.0;
const float SCREEN_YSCALE = 1.0;     // ascii characters are tall + thin, so squish the Y axis

void DrawLine(int x1, int y1, int x2, int y2)
{
    // Draw actual line
    DrawLine_SCOPE(x1, y1, x2, y2);
}

void DrawRect(int x1,int y1,int x2,int y2) {
    DrawLine(x1,y1,x2,y1);
    DrawLine(x2,y1,x2,y2);
    DrawLine(x2,y2,x1,y2);
    DrawLine(x1,y2,x1,y1);
}

void DrawGrayScale(int x1, int y1, int size, float intensity) {
    intensity = 1.0 - intensity;    // invert
    int pat  = 1000;
    int step = int(intensity * pat + 0.5);
    for ( int y = y1; y < (y1+size); y += step )
        for ( int x = x1; x < (x1+size); x += step )
            DrawLine(x,y,x,y);
    // Rect
    DrawRect(x1,y1,x1+size,y1+size);
}

void draw()
{
    // Update the screen
    //     Draw a test pattern on the scope.
    //

    // Draw circle in 0..65535 screen space
    {
        double step = 6.28 / 100.0;     // ERCO: 6.28 / 1000.0
        int x1,y1,x2=0,y2=0;
        for ( double angle = 0; angle <= 6.28; angle += step) {
            x1 = int(sin(angle) * 32767.0 + .5);   // -32767 .. 32767
            y1 = int(cos(angle) * 32767.0 + .5);
            x1 += 32767;                           // 0 .. 65535
            y1 += 32767;
            if ( angle > 0 ) DrawLine(x1,y1,x2,y2);
            x2 = x1;
            y2 = y1;
        }
    }

    // Draw "X"
    DrawLine(0,     0, 65535, 65535);
    DrawLine(0, 65535, 65535, 0);

    // Draw rectangle around screen edge
    DrawLine(0,         0, 65534,     0);
    DrawLine(65534,     0, 65534, 65534);
    DrawLine(65534, 65534,     0, 65534);
    DrawLine(0,     65534,     0,     0);

    // Draw little "+"s
    {
        int size = 1000;
        //                    X      Y          X      Y          X      Y
        float xy[][2] = { { 16384, 16384 }, { 32767, 16384 }, { 49152, 16384 },
                          { 16384, 32767 }, { 32767, 32767 }, { 49152, 32767 },
                          { 16384, 49152 }, { 32767, 49152 }, { 49152, 49152 } };
        for ( int row=0; row<9; row++ ) {
            float x=xy[row][1], y=xy[row][0];
            DrawLine(x-size,      y, x+size, y);
            DrawLine(x,      y-size, x,      y+size);
        }
    }

    // Draw an "F"
    //
    //          _________
    //         |
    //         |  a
    //         |
    //       c o-----       ..where 'o' is the origin
    //         |
    //         | b
    //         |
    //
    {
        int size = 1000;
        int x = 8192, y=32767;
        DrawLine(x, y-size, x+size*2,   y-size); // a
        DrawLine(x, y,        x+size,        y); // b
        DrawLine(x, y-size,        x,   y+size); // c
    }

    // Draw a 'grayscale'
    {
        int size = 5500;
        int x1 = 8192, y1 = 52000; // starting x/y of scale
        DrawGrayScale(x1, y1, size, 0.10); x1 += size;
        DrawGrayScale(x1, y1, size, 0.20); x1 += size;
        DrawGrayScale(x1, y1, size, 0.30); x1 += size;
        DrawGrayScale(x1, y1, size, 0.40); x1 += size;
        DrawGrayScale(x1, y1, size, 0.50); x1 += size;
        DrawGrayScale(x1, y1, size, 0.60); x1 += size;
        DrawGrayScale(x1, y1, size, 0.70); x1 += size;
        DrawGrayScale(x1, y1, size, 0.80); x1 += size;
        DrawGrayScale(x1, y1, size, 0.90); x1 += size;
    }
}

int main(int argc, char* args[])
{
    Init_SCOPE("test-pattern.wav");  // Initialize rendering
    SetStepRate_SCOPE(500);           // ERCO: step rate for line drawing (10)
    for (int i=0; i<100; i++ ) { draw(); }
    Close_SCOPE();
    return 0;
}
