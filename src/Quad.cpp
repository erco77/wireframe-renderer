// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#include "Quad.h"
#include "Point3D.h"

// ctor
Quad::Quad()
{
}

// ctor
Quad::Quad(const Point3D (&p)[4])
{
    p4[0] = p[0];
    p4[1] = p[1];
    p4[2] = p[2];
    p4[3] = p[3];
}

// ctor
Quad::Quad(const std::vector<Point3D>& pvec)
{
    p4[0] = pvec[0];
    p4[1] = pvec[1];
    p4[2] = pvec[2];
    p4[3] = pvec[3];
}

// ctor
Quad::Quad(Point3D &a, Point3D &b, Point3D &c, Point3D &d)
{
    p4[0] = a;
    p4[1] = b;
    p4[2] = c;
    p4[3] = d;
}

// copy ctor
Quad::Quad(const Quad& o)
{
    p4[0] = o.p4[0];
    p4[1] = o.p4[1];
    p4[2] = o.p4[2];
    p4[3] = o.p4[3];
}

// copy ctor
Quad::Quad(const Quad* o)
{
    p4[0] = o->p4[0];
    p4[1] = o->p4[1];
    p4[2] = o->p4[2];
    p4[3] = o->p4[3];
}

// dtor
Quad::~Quad(void)
{
}

#include <stdio.h>
void Quad::show(const char *msg) const
{
    if ( msg ) printf("%s: ", msg);
    p4[0].show(); printf(" ");
    p4[1].show(); printf(" ");
    p4[2].show(); printf(" ");
    p4[3].show();
}
