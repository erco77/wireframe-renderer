// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#include "Matrix41.h"

// ctor
Matrix41::Matrix41(const float (&m)[4][1])
{
    m41[0][0] = m[0][0];
    m41[1][0] = m[1][0];
    m41[2][0] = m[2][0];
    m41[3][0] = m[3][0];
}

// ctor
Matrix41::Matrix41(float X,float Y,float Z,float W)
{
    m41[0][0] = X;
    m41[1][0] = Y;
    m41[2][0] = Z;
    m41[3][0] = W;
}

// dtor
Matrix41::~Matrix41(void)
{
}

void Matrix41::setValue(int row, float val)
{
    m41[row][0] = val;
}

// Multiply self with a 4x4 Matrix
void Matrix41::multiply(const Matrix44& m44)
{
    float result[4][1] = { { 0.0f }, { 0.0f }, { 0.0f }, { 0.0f } };
    for (int col = 0; col < 4; col++)
        for (int row = 0; row < 4; row++)
            result[row][0] += m41[col][0] * m44.getValue(row,col);
    xyzw(result);
}

#include <stdio.h>
#include <string.h>
void Matrix41::show(const char *msg) const
{
    int i;
    char s[4][40];
    int width = 6;
    for (i=0; i<4; i++) {
        sprintf(s[i], "%.2f", m41[i][0]);   // 2 digit precision
        width = strlen(s[i]) > width ? strlen(s[i]) : width;
    }
    printf("%-10s   _ %*s _\n", msg,  width, "");
    printf("%-10s  |  %*s  |\n", "",  width, s[0]);
    printf("%-10s  |  %*s  |\n", "",  width, s[1]);
    printf("%-10s  |  %*s  |\n", "",  width, s[2]);
    printf("%-10s  |_ %*s _|\n", "",  width, s[3]);
}
