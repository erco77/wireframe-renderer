// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#include "Matrix44.h"

#pragma once
class Matrix41 {
private: 
    float m41[4][1];
public:
    Matrix41(float X,float Y,float Z,float W);
    Matrix41(const float (&m)[4][1]);
    ~Matrix41(void);
    float getValue(int row) const { return m41[row][0]; }

    void setValue(int row, float val);
    // new
    inline void x(float val) { m41[0][0] = val; }
    inline void y(float val) { m41[1][0] = val; }
    inline void z(float val) { m41[2][0] = val; }
    inline void w(float val) { m41[3][0] = val; }
    inline float x() const { return m41[0][0]; }
    inline float y() const { return m41[1][0]; }
    inline float z() const { return m41[2][0]; }
    inline float w() const { return m41[3][0]; }
    inline void xyzw(float X,float Y,float Z,float W) { x(X); y(Y); z(Z); w(W); }
    inline void xyzw(float (&a)[4][1]) { x(a[0][0]); y(a[1][0]); z(a[2][0]); w(a[3][0]); }

    // Transforms
    void multiply(const Matrix44& m44);
    void show(const char *msg="") const;
};
