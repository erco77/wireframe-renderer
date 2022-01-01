// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#pragma once
#include <vector>
#include "Point3D.h"

class Quad {
private:
    Point3D p4[4];
public:
    Quad();
    Quad(const Point3D (&p)[4]);
    Quad(const std::vector<Point3D> &pvec);
    Quad(Point3D &a, Point3D &b, Point3D &c, Point3D &d);
    Quad(const Quad& o);
    Quad(const Quad* o);
    ~Quad(void);

    Point3D& a() { return p4[0]; }   // non-const allows changes
    Point3D& b() { return p4[1]; }
    Point3D& c() { return p4[2]; }
    Point3D& d() { return p4[3]; }

    // new
    int x1() { return p4[0].x(); }
    int y1() { return p4[0].y(); }
    int x2() { return p4[1].x(); }
    int y2() { return p4[1].y(); }
    int x3() { return p4[2].x(); }
    int y3() { return p4[2].y(); }
    int x4() { return p4[3].x(); }
    int y4() { return p4[3].y(); }

    // Ways to transform a Quad
    inline void translate(const float& x, const float& y, const float& z)
    {
        p4[0].translate(x,y,z);
        p4[1].translate(x,y,z);
        p4[2].translate(x,y,z); 
        p4[3].translate(x,y,z); 
    }

    void show(const char *msg=0) const;
};
