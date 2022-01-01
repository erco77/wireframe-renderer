// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#include "DrawableObject.h"
#include "TransformHandler.h"

// ctor
DrawableObject::DrawableObject()
{
    _triangles = new vector<Triangle*>();
    _quads = new vector<Quad*>();
}

// ctor
DrawableObject::DrawableObject(const Point3D& position,
                               vector<Triangle*> *triangles)
{
    _triangles = triangles;
}

// ctor
DrawableObject::DrawableObject(const Point3D& position)
{
    _triangles = new vector<Triangle*>();
    _quads = new vector<Quad*>();
}

// copy ctor
DrawableObject::DrawableObject(const DrawableObject& o)
{
    int i;

    _triangles = new vector<Triangle*>();
    for (i=0; i<o._triangles->size(); i++)
        _triangles->push_back(new Triangle(o._triangles->at(i))); // create new copies

    _quads = new vector<Quad*>();
    for (i=0; i<o._quads->size(); i++)
        _quads->push_back(new Quad(o._quads->at(i)));             // create new copies
}

// dtor
DrawableObject::~DrawableObject(void)
{
    int i;

    for ( i=0; i<_triangles->size(); i++ )
        if ( _triangles->at(i) )
            delete _triangles->at(i);
    if (_triangles) delete _triangles;

    for ( i=0; i<_quads->size(); i++ )
        if ( _quads->at(i) )
            delete _quads->at(i);
    if (_quads) delete _quads;
}

// See if any part of object is behind camera near clipping plane
bool DrawableObject::isBehindCamera(const Camera& camera)
{
    unsigned int j;
    // Any of object's triangles behind camera?
    for (j=0; j<_triangles->size(); j++) {
        Triangle *t = _triangles->at(j);
        if (t->a().z() > -camera.getNear() ||
            t->b().z() > -camera.getNear() ||
            t->c().z() > -camera.getNear()) {
            return true;
        }
    }

    // Any of object's quads behind camera?
    for (j=0; j<_quads->size(); j++) {
        Quad *q = _quads->at(j);
        if (q->a().z() > -camera.getNear() ||
            q->b().z() > -camera.getNear() ||
            q->c().z() > -camera.getNear() ||
            q->d().z() > -camera.getNear()) {
            return true;
        }
    }
    return false;
}

// Translate all drawable object's objects along vector x,y,z
void DrawableObject::translate(const float& x, const float& y, const float& z)
{
    unsigned int j;
    for (j=0; j<_triangles->size(); j++)
        _triangles->at(j)->translate(x,y,z);
    for (j=0; j<_quads->size(); j++)
        _quads->at(j)->translate(x,y,z);
}

// Move point into camera's view space
//    Apply the look transform..
//
void DrawableObject::toViewSpace(const Camera& camera)
{
    unsigned int i;
    for ( i=0; i < triangles()->size(); i++ ) {
        Triangle *t = triangle(i);
        camera.pointToViewSpace(t->a());
        camera.pointToViewSpace(t->b());
        camera.pointToViewSpace(t->c());
    }
    for ( i=0; i < quads()->size(); i++ ) {
        Quad *q = quad(i);
        camera.pointToViewSpace(q->a());
        camera.pointToViewSpace(q->b());
        camera.pointToViewSpace(q->c());
        camera.pointToViewSpace(q->d());
    }
}

// Normalized Device Space
void DrawableObject::objectToNDC(const Camera& camera,
                                 float scrn_w,
                                 float scrn_h)
{
    float near   = camera.getNear();
    float far    = camera.getFar();
    float width  = camera.getNDCWidth();
    float height = width / (scrn_w / scrn_h);

    unsigned int i;
    for ( i=0; i<triangles()->size(); i++) {
        Triangle *t = triangle(i);
        TransformHandler::toNDC(t->a(), near, far, height, width);
        TransformHandler::toNDC(t->b(), near, far, height, width);
        TransformHandler::toNDC(t->c(), near, far, height, width);
    }
    for ( i=0; i<quads()->size(); i++) {
        Quad *q = quad(i);
        TransformHandler::toNDC(q->a(), near, far, height, width);
        TransformHandler::toNDC(q->b(), near, far, height, width);
        TransformHandler::toNDC(q->c(), near, far, height, width);
        TransformHandler::toNDC(q->d(), near, far, height, width);
    }
}

// Draw the objects using specified line drawing function
void DrawableObject::draw(LineFunc *drawLine)
{
    unsigned int i;
    for (i = 0; i < triangles()->size(); i++) {
        Triangle *t = triangle(i);
        drawLine(t->x1(), t->y1(), t->x2(), t->y2());
        drawLine(t->x2(), t->y2(), t->x3(), t->y3());
        drawLine(t->x3(), t->y3(), t->x1(), t->y1());
    }
    for (i = 0; i < quads()->size(); i++) {
        Quad *q = quad(i);
        drawLine(q->x1(), q->y1(), q->x2(), q->y2());
        drawLine(q->x2(), q->y2(), q->x3(), q->y3());
        drawLine(q->x3(), q->y3(), q->x4(), q->y4());
        drawLine(q->x4(), q->y4(), q->x1(), q->y1());
    }
}
