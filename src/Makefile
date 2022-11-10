SHELL=/bin/sh
OBJS=Matrix41.o Matrix44.o Point3D.o Triangle.o Quad.o \
     TransformHandler.o Camera.o DrawableObject.o
CXXFLAGS+=-g

# DEFINE ONLY ONE RENDER DEVICE

### FLTK RENDER DEVICE
#FLTKDIR=/usr/local/src/fltk-1.4.x-git
#FLTKCONFIG=$(FLTKDIR)/fltk-config
#CXXFLAGS+=-DRENDER_FLTK `$(FLTKCONFIG) --cxxflags`
#RENDEROBJS=fltk.o
#RENDERLIBS=`$(FLTKCONFIG) --ldstaticflags`

### FLTK CAIRO RENDER DEVICE
#FLTKDIR=/usr/local/src/fltk-1.4.x-git
#FLTKCONFIG=$(FLTKDIR)/fltk-config
#CXXFLAGS+=-DRENDER_FLTKCAIRO `$(FLTKCONFIG) --use-cairo --cxxflags`
#RENDEROBJS=fltkcairo.o
#RENDERLIBS=`$(FLTKCONFIG) --use-cairo --ldstaticflags`

### TEK4010 RENDER DEVICE
#CXXFLAGS+=-DRENDER_TEK4010
#RENDEROBJS=tek4010.o
#RENDERLIBS=

### VT100 RENDER DEVICE
### This renders ascii characters to plot pixels.
### For this to work at all, make a large console and use a tiny font
### to achieve a 600 x 150 "pixel" window
CXXFLAGS+=-DRENDER_VT100
RENDEROBJS+=vt100.o
RENDERLIBS=

### OSCILLOSCOPE/WAV FILE RENDER DEVICE
#CXXFLAGS+=-DRENDER_SCOPE
#RENDEROBJS+=scope.o
#RENDERLIBS=

all: main

#
# The main 3D application
#
main: $(OBJS) $(RENDEROBJS) main.o
	g++ $(CXXFLAGS) main.o $(OBJS) $(RENDEROBJS) $(RENDERLIBS) -o main

# Screen rendering modules
scope.o: scope.cpp scope.h
vt100.o: vt100.cpp vt100.h
tek4010.o: tek4010.cpp tek4010.h
fltk.o: fltk.cpp fltk.h
fltkcairo.o: fltkcairo.cpp fltkcairo.h

# 3D rendering modules
Quad.o: Quad.cpp Quad.h
Camera.o: Camera.cpp Camera.h
Point3D.o: Point3D.cpp Point3D.h
Matrix41.o: Matrix41.cpp Matrix41.h
Matrix44.o: Matrix44.cpp Matrix44.h
Triangle.o: Triangle.cpp Triangle.h
DrawableObject.o: DrawableObject.cpp DrawableObject.h
TransformHandler.o: TransformHandler.cpp TransformHandler.h
main.o: main.cpp

#
# Module test apps (used during module development and regression testing)
#
test-Matrix41: test-Matrix41.o Matrix41.o
	g++ $(CXXFLAGS) test-Matrix41.o Matrix41.o -o test-Matrix41

test-Matrix44: test-Matrix44.o Matrix44.o
	g++ $(CXXFLAGS) test-Matrix44.o Matrix44.o -o test-Matrix44

test-Quad: test-Quad.o Quad.o Point3D.o
	g++ $(CXXFLAGS) test-Quad.o Quad.o Point3D.o -o test-Quad

test-Triangle: test-Triangle.o Triangle.o Point3D.o
	g++ $(CXXFLAGS) test-Triangle.o Triangle.o Point3D.o -o test-Triangle

# Only builds if fltk rendering enabled
test-fltk: test-fltk.o $(RENDEROBJS)
	g++ $(CXXFLAGS) test-fltk.cpp $(RENDEROBJS) $(RENDERLIBS) -o test-fltk

# Only builds if fltkcairo rendering enabled
test-fltkcairo: $(RENDEROBJS) test-fltkcairo.o
	g++ $(CXXFLAGS) test-fltkcairo.cpp $(RENDEROBJS) $(RENDERLIBS) -o test-fltkcairo

# Test scope rendering
test-scope: scope.o test-scope.cpp
	g++ $(CXXFLAGS) test-scope.cpp scope.o -o test-scope

#
# Clean target
#
clean:
	-/bin/rm -f $(OBJS) $(RENDEROBJS) 2> /dev/null
	-/bin/rm -f main                  2> /dev/null
	-/bin/rm -f test-Quad             2> /dev/null
	-/bin/rm -f test-Triangle         2> /dev/null
	-/bin/rm -f test-fltk             2> /dev/null
	-/bin/rm -f *.o                   2> /dev/null
