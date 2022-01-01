// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#pragma once
#include <vector>
#include "Point3D.h"

class Triangle {
private:
    Point3D p3[3];
public:
    Triangle();
    Triangle(const Point3D &a, const Point3D &b, const Point3D &c);
    Triangle(const Point3D (&p)[3]);
    Triangle(const Triangle* o);
    Triangle(const Triangle& o);
    ~Triangle(void);

    Point3D& a() { return p3[0]; }    // non-const allows changes
    Point3D& b() { return p3[1]; }
    Point3D& c() { return p3[2]; }
    void a(const Point3D& p) { p3[0] = p; }
    void b(const Point3D& p) { p3[1] = p; }
    void c(const Point3D& p) { p3[2] = p; }
    void abc(const Point3D& a, const Point3D& b, const Point3D& c);

    // new
    int x1() { return p3[0].x(); }
    int y1() { return p3[0].y(); }
    int x2() { return p3[1].x(); }
    int y2() { return p3[1].y(); }
    int x3() { return p3[2].x(); }
    int y3() { return p3[2].y(); }

    // Ways to transform a triangle
    inline void translate(const float& x, const float& y, const float& z)
    {
        p3[0].translate(x,y,z);
        p3[1].translate(x,y,z);
        p3[2].translate(x,y,z); 
    }

    void show(const char *msg=0) const;
};
