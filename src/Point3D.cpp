// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#include "Point3D.h"
#include <stdio.h>

void Point3D::show(const char *msg) const
{
    if (msg) printf("%s: ", msg);
    printf("{ %8.2f, %8.2f, %8.2f }", _x, _y, _z);
}
