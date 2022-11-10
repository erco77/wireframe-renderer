Class Hierarchy
===============

main
  |__ MakeCube - Creates a normalized 3D cube of specified size
  |
  |__ DrawableObject
        |
	|__ TransformHandler
	|     |__ Matrix41, Matrix44, Camera
	|
	|__ Triangle
	|     |__ Point3D
	|
	|__ Quad
	|     |__ Point3D
	|
	|__ Camera
	      |__ Point3D

    Source Description
    ==================
    Description of the classes/source files (alphabetical order):


	Camera.cpp         \__ Camera object class.
	Camera.h           /   Near/far clipping plane, position, lookat point..

	DrawableObject.cpp \__ Manages objects made of 3D triangles or quads.
	DrawableObject.h   /   Has object methods to transform, viewspace conversion,
			       conversion to NDC (Normalize Device Space), etc.
			       relative to a specified camera.

	MakeCube.cpp       -- Creates a normalized 3D cube of specified size

	Makefile

	Matrix41.cpp \__ 4 x 1 Matrix class.
	Matrix41.h   /   Manages ( x,y,z,w ) as a class object. Set, get, multiply w/4x4.

	Matrix44.cpp \__ 4 x 4 Matrix class.
	Matrix44.h   /   Manages a 4x4 matrix transform. Set, get.

	Point3D.cpp  \__ 3D Point class.
	Point3D.h    /   Manages ( x,y,z ) as a class object. Set, get, translate.

	Quad.cpp     \__ 3D Quad class.
	Quad.h       /   A 3D quadralateral (square, rectangle, etc) with 4 vertices.
	                 Should be planar (flat), but doesn't have to be.

	TransformHandler.cpp \__ Handle common 3D transformations.
	TransformHandler.h   /   Scale/Rotate 3D points and DrawableObjects.

	Triangle.cpp  \__ 3D Triangle class.
	Triangle.h    /   A 3D triangle with 3 vertices.

	main.cpp      -- The main application that renders whatever the scene is.
	                 Currently hardwired to render a large grid.

	scope.cpp     \__ Optional Oscilloscope Driver.
	scope.h       /   Renders 3D scenes to an oscilloscope by generating a wav file
                          that contains X,Y info. The .wav can then be 'played' on an
			  oscilloscope through an 'appropriate' audio output device
			  (that doesn't do output filtering), the L and R channels
			  can drive the separate X,Y inputs on an *analog* oscilloscope.

     OUTPUT DRIVERS
     --------------

	tek4010.cpp  \__ Optional Tektronix 4010 terminal output driver.
	tek4010.h    /   Renders 3D scenes using Tek4010 codes that can be simply 'cat'ed
	                 while in a tek4010 terminal (or emulator).

	vt100.cpp    \__ Optional VT100 terminal output driver.
	vt100.h      /   Renders 3D scenes using VT100 escape codes to plot 'pixels' using
                         ASCII art. This is useful for debugging, esp on xterm (or similar)
			 windows with the font size set really small.

	fltk.cpp    \__ Optional FLTK output driver.
	fltk.h      /   Renders 3D scenes using an FLTK window.
                        FLTK is a graphics toolkit that supports rudimentary line drawing.

	fltkcairo.cpp \__ Optional Cairo output driver.
	fltkcairo.h   /   Renders 3D scenes using Cairo's antialiased line drawing tools.

	framestamp
	images
	notes.txt
	shoot
	tech-notes.txt
	tomovie

      TEST PROGRAMS
      -------------
      These standalone programs test the various classes for regression testing.

	test-Matrix41.cpp  -- Creates some 4x1 matrices and prints them.
	test-Matrix44.cpp  -- Creates some 4x4 matrices and prints them.
	test-Quad.cpp      -- Creates quad objects in various ways and prints them.
	test-Triangle.cpp  -- Creates triangle objects in various ways and prints them.
	test-fltk.cpp      -- Tests the FLTK output driver by drawing an 'X'
	test-fltkcairo.cpp -- Tests the FLTK+Cairo output driver by drawing an 'X'
	test-scope.cpp     -- Tests the oscilloscope output driver by drawing an 'X'
