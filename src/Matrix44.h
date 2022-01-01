// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#pragma once
class Matrix44 {
private: 
    float m44[4][4];
public:
    Matrix44(float m[4][4]);
    Matrix44(const float& m00, const float& m01, const float& m02, const float& m03,
             const float& m10, const float& m11, const float& m12, const float& m13,
             const float& m20, const float& m21, const float& m22, const float& m23,
             const float& m30, const float& m31, const float& m32, const float& m33);
    ~Matrix44(void) { }
    inline float getValue(int row, int col) const { return m44[row][col]; }
    inline void setValue(int row, int col, float val) { m44[row][col] = val; }
    void show(const char *msg="") const;
};
