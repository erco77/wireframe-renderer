// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#pragma once
class Point3D {
    float _x;   // underbars allow methods named e.g. x()
    float _y;
    float _z;

public:
    // ctors
    Point3D(const float& X, const float& Y, const float& Z) { _x = X; _y = Y; _z = Z; }
    Point3D() { _x = _y = _z = 0.0f; }

    // copy ctor
    Point3D(const Point3D& o) { _x = o._x; _y = o._y; _z = o._z; }

    // dtor
    ~Point3D(void) { }

    // assignment
    Point3D& operator=(const Point3D& o) { _x = o._x; _y = o._y; _z = o._z; return *this; }

    inline float x() const { return _x; }
    inline float y() const { return _y; }
    inline float z() const { return _z; }
    inline void x(const float& X) { _x = X; }
    inline void y(const float& Y) { _y = Y; }
    inline void z(const float& Z) { _z = Z; }
    inline void xyz(const float& X, const float& Y, const float& Z) { _x = X; _y = Y; _z = Z; }

    // Ways to transform a point
    inline void translate(const float& x, const float& y, const float& z) { _x += x; _y += y; _z += z; }

    void show(const char *msg=0) const;
};
