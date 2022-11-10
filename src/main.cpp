// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

//
// 3D Engine - A 3D object wireframe renderer 
//
// Much thanks to: https://github.com/njustesen/3D-Engine
//
// 1.0 - Nov 2012 njustesen @ github - Initial implementation
// 1.1 - Dec 25 2021 erco@seriss.com - Cleanup, removed SDL, added output driver for tek4010 rendering
// 1.2 - Jan 01 2022 erco@seriss.com - More cleanup, added more output drivers: vt100, fltk, fltkcairo
// 1.3 - Jan 01 2022 erco@seriss.com - Added animation features
//
// There's a good tektronix 4010 emulator (used for initial development): 
// https://github.com/rricharz/Tek4010
// ..in addition to the regular tek4010 driver in xterm: Ctrl-Middle click, "Switch to Tek mode"
//

#include "DrawableObject.h"
#include "TransformHandler.h"
#include "Camera.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>     // cosf()
#include <unistd.h>   // usleep()

// These are defined by the Makefile
// to ensure the proper libraries are linked
//
//#define RENDER_VT100
//#define RENDER_TEK4010
//#define RENDER_FLTK
//#define RENDER_FLTKCAIRO
//..etc..

#ifdef RENDER_SCOPE
    #include "scope.h"
    //
    // Oscilloscope/WAV File Rendering - Renders vectors to .wav file
    //                                   Note: the screen space is HUGE; full signed 16bit numbers.
    //
    #define     SCREEN_WIDTH    1000    // Lets use unsigned 0 .. 65535
    #define     SCREEN_HEIGHT   1000    // since we're used to this.
    #define     SCREEN_XOFF     0
    #define     SCREEN_YOFF     0   // -32767
#endif

#ifdef RENDER_VT100
    #include "vt100.h"
    //
    // VT100 Rendering - Renders scene in "ascii art".
    //                   Works "well" with tiny font + large window size (e.g. 600x150)
    //                   Impractical for realtime animation (too lores, slow, etc)
    //
    #define     SCREEN_WIDTH    VT100_SCREEN_XMAX       // set in vt100.h
    #define     SCREEN_HEIGHT   VT100_SCREEN_YMAX 
    #define     SCREEN_XSCALE   1.0 
    #define     SCREEN_YSCALE   0.3                     // ascii characters are tall + thin, so squish the Y axis
    #define     SCREEN_XOFF     0   // 200 
    #define     SCREEN_YOFF     0   // 50 
#endif

#ifdef RENDER_TEK4010
    #include "tek4010.h"
    //
    // TEK4010 Terminal - Renders scene on a 1970's "Storage Tube" terminal.
    //                    Looks cool during rendering using emulator: https://github.com/rricharz/Tek4010
    //                    Also works in xterm: (a) open 'xterm', (b) Ctrl-Middle menu, choose "Switch to Tek Mode"
    //                    Impractical for realtime animation (too slow, flash between frames).
    //              
    const int   SCREEN_WIDTH  = TEK4010_SCREEN_XMAX;    // set in tek4010.h
    const int   SCREEN_HEIGHT = TEK4010_SCREEN_YMAX;
    const float SCREEN_XSCALE = 1.0;
    const float SCREEN_YSCALE = 1.0;
    const int   SCREEN_XOFF   = 0;
    const int   SCREEN_YOFF   = 50;
#endif

    // I YAM HERE - CHANGE EVERYTHING TO USE #define's
    //              SO WE CAN CHECK REALTIME SIZE OF WINDOW, ETC

#ifdef RENDER_FLTK
    #include "fltk.h"
    //
    // FLTK Rendering - Realtime rendering in FLTK
    //                  Can be used for TD'ing animation, can easily render 30fps and higher..
    //
    #define SCREEN_WIDTH   FLTK_SCREEN_XMAX       // set in fltk.h
    #define SCREEN_HEIGHT  FLTK_SCREEN_YMAX
    // NO SCREEN_XSCALE
    // NO SCREEN_YSCALE
    #define SCREEN_XOFF    0
    #define SCREEN_YOFF    0
#endif

#ifdef RENDER_FLTKCAIRO
    #include "fltkcairo.h"
    //
    // FLTK CAIRO Rendering - Realtime rendering in FLTK
    //                        Can be used for TD'ing animation, can easily render 30fps and higher..
    //
    const int   SCREEN_WIDTH  = FLTKCAIRO_SCREEN_XMAX;       // set in fltkcairo.h
    const int   SCREEN_HEIGHT = FLTKCAIRO_SCREEN_YMAX;
    const float SCREEN_XSCALE = 1.0;
    const float SCREEN_YSCALE = 1.0;
    const int   SCREEN_XOFF   = 0;
    const int   SCREEN_YOFF   = 0;
#endif

using namespace std;

// MODULE GLOBALS
static Camera camera;                      // the camera object
static vector<DrawableObject*> *objects;   // 'world space' objects
static DrawableObject *lookat = 0;         // lookat cube

// Defines MakeCube()
#include "MakeCube.cpp"

// Initialize our 3D stuff
void Init_3D()
{
    // Initialize Camera
    //
    //           Near Z    Far Z
    //             :          :  .
    //             :          :.`
    //             :         .:
    //             :       .` :
    //             :     .`   :
    //             :   .`     :
    //             : .`       :
    //             :`         :
    //   _____   .`:          :
    //  |     |/|  : Scene    :   ___  _  ___  _  ___  Camera Z
    //  |_____|\|  : Objects  :                        Axis
    //           `.:          :
    //   Camera    :.         :
    //             : `.       :
    //             :   `.     :
    //             :     `.   :
    //             :       `. :
    //             :         `:
    //             :          :`.
    //             :          :  `
    //
    // WARNING: Avoid gimble lock!!! -- Don't look STRAIGHT UP or STRAIGHT DOWN on Y AXIS!
    // WARNING: Avoid gimble lock!!! -- Don't look STRAIGHT UP or STRAIGHT DOWN on Y AXIS!
    // WARNING: Avoid gimble lock!!! -- Don't look STRAIGHT UP or STRAIGHT DOWN on Y AXIS!
    //

    // TODO: SOMETHING IS WRONG WITH LOOKAT
    //       Seems like actual lookat position is opposite (on all axes)
    //       from where it is. To show a lookat object, it has to be located
    //       on the NEGATIVE x,y,z of the lookat point.
    //
    //     What works: camera pos: 0,0,+1           
    //                    look at: 0,0,-100              o -->  o          o
    //                        obj: 0,0,5000             cam   lookat      obj
    //                                                   1     -1        -1000
    //
    //                                                Camera looking down -Z axis
    //
    camera.setPosition(   0.0f,    0.0f,    1.0f);  // camera position    z:smaller zooms in
    camera.setLookAt(  -290.0f, -290.0f, -100.0f);  // camera look at
    camera.setHorizontalViewAngle(60.0f);           // camera horizontal viewing angle
    camera.setNear(10);                             // camera near z clip
    camera.setFar(20000);                           // camera far z clip (XXX not currently implemented)

    printf("--- CAMERA INITIALIZED\n");
    
    // Add objects
    objects = new vector<DrawableObject*>();

    // Create grid
    //     I used 1000 for the squaresize value for the first neon render.
    //
    #define SQUARESIZE 500
    #define SQZ2 (SQUARESIZE/2.0)
    {
        // Make a single 100x100 square as a quad on the XY plane
        //    It's built around the model space's origin:
        //
        //
        //                _________ (50,50)
        //               |         |
        //               |         |
        //               |    o    |  <-- (0,0)
        //               |         |
        //               |_________|
        //      (-50,-50)
        //
        DrawableObject square;
        {
            Point3D p0(-SQZ2, -SQZ2, 0.0);
            Point3D p1( SQZ2, -SQZ2, 0.0);
            Point3D p2( SQZ2,  SQZ2, 0.0);
            Point3D p3(-SQZ2,  SQZ2, 0.0);
            square.addQuad(new Quad(p0,p1,p2,p3));
        }


        // Now make copies of the quad all over a grid in world space
        //    I used 60000 for the final neon renders (first animation test)
        //
        for ( int x=-30000; x<30000; x+=SQUARESIZE ) {
            for ( int y=-30000; y<30000; y+=SQUARESIZE ) {
                DrawableObject *qcopy = new DrawableObject(square);  // instance the square
                qcopy->translate(x, y,  3000);                       // -z puts object out in front of camera
                objects->push_back(qcopy);
            }
        }

    }

    // Create a 'lookat' cube LAST
    lookat = MakeCube(5.0f); // x,y,z,cube_diameter
    lookat->translate(-camera.lookAt().x(),     // XXX why negative on ALL AXES to make it visible?
                      -camera.lookAt().y(),
                      -camera.lookAt().z());
    objects->push_back(lookat);


        // TODO: Make a special square mesh object (or wavefront object)
        //       where a grid of points can be defined, followed by connectivity.
        //       This way lines will be drawn ONCE between points, e.g.
        //
        //       .__.__.__.__.__.__.__.__.__.  <-- a single 'polyline' (unclosed polygon)
        //           
        //       .  .  .  .  .  .  .  .  .  .
        //
        //       .  .  .  .  .  .  .  .  .  .
        //
        //       .  .  .  .  .  .  .  .  .  .
        //
        //       This avoids typical mesh drawing where an edge
        //       might be drawn twice where squares are coincident.
        //       This will avoid antialiased lines over each other
        //       (making double thickness) and/or problems when using
        //       a real vector display where a vector overdraws
        //
}

void DrawLine(int x1, int y1, int x2, int y2)
{
    //printf(" PRECLIP: %d, %d, %d, %d\n", x1, y1, x2, y2);
#ifdef SCREEN_XSCALE
    // Apply scale (if render driver defined any)
    x1 *= SCREEN_XSCALE; y1 *= SCREEN_YSCALE;
    x2 *= SCREEN_XSCALE; y2 *= SCREEN_YSCALE;
#endif

    // Apply offset
    x1 += SCREEN_XOFF; y1 += SCREEN_YOFF;
    x2 += SCREEN_XOFF; y2 += SCREEN_YOFF;

    // Full line clip
    if ( x1 < 0 && x2 < 0 ) return;
    if ( y1 < 0 && y2 < 0 ) return;
    if ( x1 > SCREEN_WIDTH  && x2 > SCREEN_WIDTH  ) return;
    if ( y1 > SCREEN_HEIGHT && y2 > SCREEN_HEIGHT ) return;

    // Draw actual line
    //    These functions are to handle window clipping lines themselves.
    //
    #if RENDER_SCOPE
    DrawLine_SCOPE(x1, y1, x2, y2);
    #endif
    #if RENDER_VT100
    // Flash-render lines
    //printf("\033[1m"); DrawLine_VT100(x1, y1, x2, y2); fflush(stdout); usleep(50000); printf("\033[0m");
    //DrawLine_VT100(x1, y1, x2, y2); fflush(stdout);

    // Regular drawing
    DrawLine_VT100(x1, y1, x2, y2);
    #endif
    #if RENDER_TEK4010
    DrawLine_TEK4010(x1, y1, x2, y2);
    #endif
    #if RENDER_FLTK
    DrawLine_FLTK(x1, y1, x2, y2);
    #endif
    #if RENDER_FLTKCAIRO
    DrawLine_FLTKCAIRO(x1, y1, x2, y2);
    #endif
}

void draw()
{
    // Update the screen
    printf("draw() called\n");

    #ifdef RENDER_VT100
    Clear_VT100();
    #endif

    #ifdef RENDER_TEK4010
    Clear_TEK4010();
    #endif

    #ifdef RENDER_FLTK
    // do nothing, screen automatically cleared
    #endif

    #ifdef RENDER_FLTKCAIRO
    // do nothing, screen automatically cleared
    #endif

    // Draw objects
    //    TODO Need a 'node' class we can put DrawableObject's into
    //         so the node can be told to 'draw' itself and everything in it.
    //
    //    TODO Also, should probably have some top level class that handles
    //         rendering the entire scene, so the ViewSpace/NDC stuff is all
    //         handled by that.
    //
    unsigned int i;
    static bool once = true;
    unsigned int find = 1;
    for (i=0; i<objects->size(); i++) {

        // Make copy of DrawableObject so we can transform it for rendering
        DrawableObject *clone = new DrawableObject(*objects->at(i));

        // Apply camera location transform
        clone->translate(-camera.x(), -camera.y(), -camera.z());    // simple addition to all verts
        
        // Camera Look Transform
        clone->toViewSpace(camera);
        
        // Clip object if behind camera
        if ( clone->isBehindCamera(camera) ) { delete clone; continue; }

        // Perspective Transform
        TransformHandler::objectToNDC(clone, camera, SCREEN_WIDTH, SCREEN_HEIGHT);

        // Move to screen space
        TransformHandler::objectToScreenSpace(clone, SCREEN_WIDTH);
        
        // Now draw the object as screen coordinates
        clone->draw(DrawLine);      // use our DrawLine() function to draw lines

        delete clone;
        if ( i == find ) { once = false; }
    }


    {
        //// Move lookat a little each frame
        static float xadj = 2;
        static float yadj = 1;

        // Handle moving around grid
        static int count = 0;
        ++count;
        if ( (count & 0x300) == 0x000 ) { xadj =  2; yadj =  1; }
        if ( (count & 0x300) == 0x100 ) { xadj = -2; yadj =  1; }
        if ( (count & 0x300) == 0x200 ) { xadj =  0; yadj = -2; }
        if ( (count & 0x300) == 0x300 ) { xadj =  2; yadj =  1; count = 0; }

        camera.xyzLookAtTranslate(xadj,yadj,0);
        lookat->translate(       -xadj,-yadj,0);    // XXX moving cube in opposite direction works?!
        //camera.lookAt().show("look"); printf("\n");
        static char once = 0;
        if ( !once ) { once = 1; printf("Animating lookat on x,y step: %.2f %.2f\n", xadj, yadj); }
    }
}

#ifdef RENDER_SCOPE
int main(int argc, char* args[])
{
    Init_3D();              // Initialize 3D stuff
    Init_SCOPE("out.wav");  // Initialize rendering
    SetStepRate_SCOPE(400); // step rate for line drawing
    // while (1) { draw(); }
    draw();     // draw 1 frame, exit
    return 0;
}
#endif

#ifdef RENDER_VT100
int main(int argc, char* args[])
{
    Init_3D();          // Initialize 3D stuff
    init_VT100();       // Initialize rendering
    //draw();           // draw once
    while (1) { draw(); fflush(stdout); usleep(100000); } // animate
    return 0;
}
#endif

#ifdef RENDER_TEK4010
int main(int argc, char* args[])
{
    Init_3D();         // Initialize 3D stuff
    init_TEK4010();    // init the tek4010
    draw();            // Draw objects
    return 0;
}
#endif

#ifdef RENDER_FLTK
int main(int argc, char* args[])
{
    Init_3D();         // Initialize 3D stuff
    Init_FLTK(draw);   // init fltk (creates window)
//XXX    Redraw_FLTK(30.0); // enable redrawing (for animation) in FPS
//XXX    SetPostRenderCommand_FLTK("./shoot");
    AppLoop_FLTK();    // FLTK uses an event loop for showing window + calling draw()
    return 0;
}
#endif

#ifdef RENDER_FLTKCAIRO
int main(int argc, char* args[])
{
    Init_3D();              // Initialize 3D stuff
    Init_FLTKCAIRO(draw);   // init fltk (creates window)
    Redraw_FLTKCAIRO(30.0); // enable redrawing (for animation) in FPS
    AppLoop_FLTKCAIRO();    // FLTK uses an event loop for showing window + calling draw()
    return 0;
}
#endif
