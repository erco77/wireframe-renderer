// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#include <stdio.h>
#include <stdlib.h>
#define ABS(x) ((x)<0?-(x):(x))
#define SGN(x) ((x)<0?-1:1)

// Module Globals
static FILE       *G_wav_fp       = 0;
static const char *G_wav_filename = 0;
static int         G_step         = 5;	// reasonable default determined empirically (400)

// Draw a line between two points on the oscilloscope - erco@seriss.com
//
//    The oscilloscope's X and Y axis for moving the beam's point
//    around the screen is controlled by +/- voltages. Which makes it
//    perfect to be controlled by either "Line" or "Headphone" outputs
//    e.g. from a computer's audio output.
//
//    This library supports the logical screen drawing area:

//             top/left
//             (0,65535)                      (65535,65535)
//             o---------------------------------o
//             |                                 |
//             |                                 |
//             |                                 |
//             |                                 |
//             |                +                |
//             |                center           |
//             |                (32767,32767)    |
//             |                                 |
//             o_________________________________|
//            (0,0)                           (65535,0)
//                                             bottom/right
//
//    When driven with 16 bit wave files, the scope's
//    actual hardware, in "X/Y mode" is mapped this way:
//
//      top/left
//      (-32767,32767)                        (32767,32767)
//             o---------------------------------o
//             |                                 |
//             |                                 |
//             |                                 |
//             |                                 |
//             |                +                |
//             |                 origin          |
//             |                 (0,0)           |
//             |                                 |
//             o_________________________________|
//      (-32767,-32767)                       (32767,-32767)
//                                             bottom/right
//
//    The voltage output of a 0db sine wave on "line out" would be around +/- 1.1V:
//
//     VOLTAGE                 - AUDIO LINE OUT -                         WAV VALUE
//     --------                                                           ---------
//
//      1.1v  -|         ,-'''-.                                    |- (+32767) 0x7fff
//             |      ,-'       `-.                                 |
//             |    ,'             `.                               |
//             |  ,'                 `.                             |
//             | /                     \                            |
//             |/                       \                           |
//        0v  -+-------------------------\---------------------------- (0)      0x0000
//             |                          \                       / |
//             |                           \                     /  |
//             |                            `.                 ,'   |
//             |                              `.             ,'     |
//             |                                `-.       ,-'       |
//     -1.1v  -|                                   `-,,,-'          |- (-32767) 0x8001
//
//    Peak voltage for a +4db signal is around +/-1.7v.
//
//    If you sent a full voltage swing sine/cosine waves to the X and Y axes
//    on the scope, you'd get a perfect circle filling the screen, touching
//    all four edges of the scope's graticule.
//
//    This would be a wav file where the L/R channels would have values
//    ranging from -32767 to +32767 (0x8001 to 0x7fff). e.g.
//
//    Computer                                               Oscilloscope
//  ___________                                __________________________________
//             |                              |             ||      .......      |
//      Right  |----------------------------> |  X Input    ||    ,`       `,    |
//        Gnd  |----------------------------> |             ||   :           :   |
//             |                              |             ||  :             :  |
//             |                              |             ||  :      +      :  |
//       Left  |----------------------------> |  Y Input    ||  :             :  |
//        Gnd  |----------------------------> |             ||   `.         .`   |
//             |                              |             ||     `.......`     |
//  ___________|                              |_____________||___________________|
//

// Swap two integers
static void swap(int &a, int &b) {
  int swap = a;
  a = b;
  b = swap;
}

// Draw a line from x1,y1 to x2,y2 stepping by 'step'
//
//     The step rate is needed because the resolution of the scope may be
//     too high for practical purposes of line drawing (too slow).
//
//     Used here: Bresenhams from Graphics Gems I: DigitalLine.c
//
static void DrawLine(int x1, int y1,
	             int x2, int y2,
	             int step,                      // TODO!
	             void (*drawfunc)(int,int))
{
    //DEBUG fprintf(stderr, "DrawLine: %d,%d -> %d,%d\n", x1,x2,y1,y2);

    // Plot single point?
    if ( x1 == x2 && y1 == y2 ) { drawfunc(x1, y1); return; }

    int d, x, y, ax, ay, sx, sy, dx, dy;
    int skip = 0;
    dx = x2-x1;  ax = ABS(dx)<<1;  sx = SGN(dx);
    dy = y2-y1;  ay = ABS(dy)<<1;  sy = SGN(dy);
    x = x1;
    y = y1;
    if (ax>ay) {                /* x dominant */
        d = ay-(ax>>1);
        for (;;) {
            if ( ++skip % step == 0 ) drawfunc(x, y);
            if (x==x2) return;
            if (d>=0) {
                y += sy;
                d -= ax;
            }
            x += sx;
            d += ay;
        }
    } else {                      /* y dominant */
        d = ax-(ay>>1);
        for (;;) {
            if ( ++skip % step == 0 ) drawfunc(x, y);
            if (y==y2) return;
            if (d>=0) {
                x += sx;
                d -= ay;
            }
            y += sy;
            d += ax;
        }
    }
}

// WRITE FIXED WAV HEADER
//    48KHZ
//    2 CHANNEL
//    2 BYTES PER SAMPLE
//
static int WriteWavHeader(FILE *fp)
{
    /**** WAV HEADER

    00000000: 52 49 46 46 24 78 e0 01 57 41 56 45 66 6d 74 20  RIFF$x..WAVEfmt
	      ----------- ----------- ----------- -----------
	      "RIFF"      chunksize   "WAVE"      "fmt "

    00000010: 10 00 00 00 01 00 02 00 80 bb 00 00 00 ee 02 00  ................
	      ----------- ----- ----- ----------- -----------
	      subchunk#1  1=PCM chans samprate    bytes/sec

    00000020: 04 00 10 00 64 61 74 61 00 78 e0 01 ff ff 00 80  ....data.x......
	      ----- ----- ----------- -----------  ^
	      align bits  "data"      subchunk#2  /|\
		    per                            |
		    sample                         DATA START

    ****/

    // WRITE 4 BYTE "FILE FORMAT MAGIC"
    //    NOTE: RIFF = little endian
    //          RIFX = big endian
    //
    fwrite("RIFF", 1, 4, fp);

    // WRITE 4 BYTE "CHUNKSIZE"
    fwrite("\x24\x78\xe0\x01", 1, 4, fp);

    // WRITE 4 BYTE "DATA FORMAT"
    fwrite("WAVE", 1, 4, fp);

    // WRITE 4 BYTE "SUBCHUNK MAGIC"
    fwrite("fmt ", 1, 4, fp);

    // WRITE 4 BYTE "SUBCHUNK #1 SIZE"
    fwrite("\x10\x00\x00\x00", 1, 4, fp);

    // WRITE 2 BYTE "AUDIO ENCODING FORMAT"
    fwrite("\x01\x00", 1, 2, fp);

    // WRITE 2 BYTE "NUMBER OF CHANNELS"
    fwrite("\x02\x00", 1, 2, fp);

    // WRITE 4 BYTE "SAMPLE RATE"
    fwrite("\x80\xbb\x00\x00", 1, 4, fp);

    // WRITE 4 BYTE "BYTE RATE"
    //    Bytes per second. Same as: bytespersamp * rate * chans
    //
    fwrite("\x00\xee\x02\x00", 1, 4, fp);

    // WRITE 2 BYTE "BLOCK ALIGN"
    fwrite("\x04\x00", 1, 2, fp);

    // WRITE 2 BYTE "BITS PER SAMPLE"
    fwrite("\x10\x00", 1, 2, fp);

    // WRITE 4 BYTE "DATA MAGIC"
    fwrite("data", 1, 4, fp);

    // WRITE 4 BYTE "SUBCHUNK #2 SIZE"
    fwrite("\x00\x78\xe0\x01", 1, 4, fp);

    return 0;
}

static void PlotToWAV(int x, int y)
{
    //DEBUG fprintf(stderr, "  PlotToWAV(%d, %d) - ",x,y);
    // Convert 0..65535 -> -32767..32767
    x -= 32767;
    y -= 32767;
    //DEBUG fprintf(stderr, "%d, %d\n", x, y);

    // Clip
    if ( x < -32767 || x > 32767 || y < -32767 || y > 32767 ) return;

    signed short s;
    s = -y; fwrite(&s, sizeof(short), 1, G_wav_fp);
    s =  x; fwrite(&s, sizeof(short), 1, G_wav_fp);
}

void DrawLine_SCOPE(int x1,int y1,int x2,int y2)
{
    DrawLine(x1, y1, x2, y2, G_step, PlotToWAV);
}

// Set the stepping rate for wav file values (used for drawing lines on scope)
//
//     Higher numbers decreases the "resolution" of the lines on the scope, but
//     speeds up the drawing speed.
//
//     Too high resolution (lower numbers) may draw smoothly, but too slowly.
//     Too low resolution (higher numbers) may draw quickly, but cause lines to break up into points.
//
//     The best value for 'step' is determined empirically where you draw as quickly
//     as possible (higher values) but never see points (lower values). In my tests,
//     a value of 400 seemed to work well for realtime speed.
//     Lower values would be better if filming off the scope with long exposures
//     with beam intensity set LOW to maximize line sharpness/resolution, e.g.
//     Open shutter, play wav file, close shutter.
//
//
void SetStepRate_SCOPE(int step)
{
    G_step = step;
}

void Close_SCOPE()
{
    if ( G_wav_fp ) {
        printf("*** Closing wav file: %s\n", G_wav_filename);
	fclose(G_wav_fp);
    }
    G_wav_fp = 0;
}

void Init_SCOPE(const char *wav_filename)
{
    G_wav_filename = wav_filename;
    G_wav_fp       = fopen(G_wav_filename, "w");
    if ( !G_wav_fp ) {
        perror(wav_filename);
	exit(1);
    }
    // Write 2 channel 16 bit-per-channel WAV header
    WriteWavHeader(G_wav_fp);

    // From here, we can just send two 16bit binary values go G_wav_fp as needed
    // to "draw" graphics on scope..
}

