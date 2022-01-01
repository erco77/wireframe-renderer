// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#include "Triangle.h"
#include "Point3D.h"

// ctor
Triangle::Triangle() { }

// ctor
Triangle::Triangle(const Point3D &a, const Point3D &b, const Point3D &c)
{
    p3[0] = a;
    p3[1] = b;
    p3[2] = c;
}

// ctor
Triangle::Triangle(const Point3D (&p)[3])
{
    p3[0] = p[0];
    p3[1] = p[1];
    p3[2] = p[2];
}

// copy ctor
Triangle::Triangle(const Triangle& o)
{
    p3[0] = o.p3[0];
    p3[1] = o.p3[1];
    p3[2] = o.p3[2];
}

// copy ctor
Triangle::Triangle(const Triangle* o)
{
    p3[0] = o->p3[0];
    p3[1] = o->p3[1];
    p3[2] = o->p3[2];
}

// dtor
Triangle::~Triangle(void)
{
}

void Triangle::abc(const Point3D& a, const Point3D& b, const Point3D& c)
{
    p3[0] = a;
    p3[1] = b;
    p3[2] = c;
}

#include <stdio.h>
void Triangle::show(const char *msg) const
{
    if ( msg ) printf("%s: ", msg);
    p3[0].show(); printf(" ");
    p3[1].show(); printf(" ");
    p3[2].show();
}
