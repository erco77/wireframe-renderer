// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#include "Matrix44.h"

// ctor
Matrix44::Matrix44(float mat[4][4])
{
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            m44[row][col] = mat[row][col];
}

// ctor
Matrix44::Matrix44(const float& m00, const float& m01, const float& m02, const float& m03,
                   const float& m10, const float& m11, const float& m12, const float& m13,
                   const float& m20, const float& m21, const float& m22, const float& m23,
                   const float& m30, const float& m31, const float& m32, const float& m33)
{
    m44[0][0] = m00; m44[0][1] = m01; m44[0][2] = m02; m44[0][3] = m03;
    m44[1][0] = m10; m44[1][1] = m11; m44[1][2] = m12; m44[1][3] = m13;
    m44[2][0] = m20; m44[2][1] = m21; m44[2][2] = m22; m44[2][3] = m23;
    m44[3][0] = m30; m44[3][1] = m31; m44[3][2] = m32; m44[3][3] = m33;
}

#include <stdio.h>
#include <string.h>
void Matrix44::show(const char *msg) const
{
    int mw = -strlen(msg);
    int r,c;
    char s[4][4][60];
    int width[4] = { 2, 2, 2, 2 };  // max widths for each column
    for (r=0; r<4; r++) {
        for (c=0; c<4; c++) {
            sprintf(s[r][c], "%.2f", m44[r][c]);   // 2 digit precision
            width[c] = (strlen(s[r][c]) > width[c]) ? strlen(s[r][c]) : width[c];
        }
    }
    printf("%*s   _ %*s  %*s  %*s  %*s _\n", mw, "",   width[0], "",
                                                       width[1], "",
                                                       width[2], "",
                                                       width[3], "");
    printf("%*s  |  %*s, %*s, %*s, %*s  |\n", mw, "",  width[0], s[0][0],
                                                       width[1], s[0][1],
                                                       width[2], s[0][2],
                                                       width[3], s[0][3]);
    printf("%*s  |  %*s, %*s, %*s, %*s  |\n", mw, msg, width[0], s[1][0],
                                                       width[1], s[1][1],
                                                       width[2], s[1][2],
                                                       width[3], s[1][3]);
    printf("%*s  |  %*s, %*s, %*s, %*s  |\n", mw, "",  width[0], s[2][0],
                                                       width[1], s[2][1],
                                                       width[2], s[2][2],
                                                       width[3], s[2][3]);
    printf("%*s  |_ %*s, %*s, %*s, %*s _|\n", mw, "",  width[0], s[3][0],
                                                       width[1], s[3][1],
                                                       width[2], s[3][2],
                                                       width[3], s[3][3]);
}
