// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#include "Camera.h"
#include "Point3D.h"
#include <vector>
#include <math.h>

// ctor
Camera::Camera()
{
    // XXX Not sure yet what good defaults should be.
    //     We should be able to look at a unit quad and see it
    //     by setting lookat to 0,0,0 and moving camera back on Z,
    //     but that's currently not working yet as of this writing.
    //
    _position.xyz(0.0f, 0.0f, -200.0f);
    _lookAt.xyz(0.0f, 0.0f, 0.0f);
    _horizontalViewAngle = 65;       // slightly wide angle
    _near = 10;
    _far  = 10000;                  // XXX not used currently
    Recalc();
}

// ctor
Camera::Camera(const Point3D& position,
               const Point3D& lookAt,
               float horizontalViewAngle,
               float near,
               float far)
{
    _position = position;
    _lookAt = lookAt;
    _horizontalViewAngle = horizontalViewAngle;
    _near = near;
    _far = far;
    Recalc();
}

// dtor
Camera::~Camera(void)
{
}

// Recalc precomp_xxx values whenever a value is changed.
void Camera::Recalc()
{
    // getUpVector()
    precomp_up_vector.xyz(0.0f, 1.0f, 0.0f);

    // getViewPlaneNormal()
    {
        // Direction from camera's eye to the lookat point as a distance vector -erco
        Point3D direction(_lookAt.x() - _position.x(),
                          _lookAt.y() - _position.y(),
                          _lookAt.z() - _position.z());

        float length = sqrt((direction.x() * direction.x()) +
                            (direction.y() * direction.y()) +
                            (direction.z() * direction.z()));

        precomp_view_plane_normal.xyz(direction.x() / length,
                                      direction.y() / length,
                                      direction.z() / length);
    }

    // getIntermediateOrthogonalAxis()
    {
        Point3D up = getUpVector();
        Point3D n  = getViewPlaneNormal();

        Point3D cross(up.y() * n.z() - up.z() * n.y(),
                      up.z() * n.x() - up.x() * n.z(),
                      up.x() * n.y() - up.y() * n.x());

        float length = sqrt((cross.x() * cross.x()) +
                            (cross.y() * cross.y()) +
                            (cross.z() * cross.z()));

        precomp_int_ortho_axis.xyz(cross.x() / length,
                                   cross.y() / length,
                                   cross.z() / length);
    }

    // getNewYAxis()
    {
        Point3D n = getViewPlaneNormal();
        Point3D u = getIntermediateOrthogonalAxis();

        float x = n.y() * u.z() - n.z() * u.y();
        float y = n.z() * u.x() - n.x() * u.z();
        float z = n.x() * u.y() - n.y() * u.x();

        precomp_new_y_axis.xyz(x, y, z);
    }

    // getHorizontalViewAngleRadians()
    {
        precomp_horizontalViewAngleRadians = getHorizontalViewAngle() * M_PI/180;   // deg -> radians
    }

    // getNDCWidth()
    {
        precomp_NDCWidth = (tan(precomp_horizontalViewAngleRadians / 2.0) * _near) * 2.0;
    }
}

#include "Matrix41.h"
#include "Matrix44.h"
#include <stdio.h>

// Move point into view space
//
// XXX Something is maybe wrong here.. I think lookat has its xyz inverted
//     It rotates the view at an odd angle.
//     Check against: https://docs.microsoft.com/en-us/windows/uwp/graphics-concepts/view-transform
//     Also: https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/lookat-function
//
//     M = Model Matrix
//     V = View Matrix (moves world into view for clipping)
//     P = Projection Matrix
//
//     So the concatenated world (or model), view, projection matrix [MVP]
//     is made by multiplying the three matrices together:
//
//         [MVP] = [P] * [V] * [M]
//
//     ..and vertex 'v' can be transformed into clip space by multiplying
//     the combined matrix [MVP]:
//
//          v       = [MVP] * v
//           (clip)            (model)
//
void Camera::pointToViewSpace(Point3D& p, bool showMatrix) const
{
    Point3D u = getIntermediateOrthogonalAxis();  // New X
    Point3D v = getNewYAxis();                    // New Y
    Point3D n = getViewPlaneNormal();             // New Z

    // efficient
    Matrix44 m44( u.x(), u.y(), u.z(), 0.0f,
                  v.x(), v.y(), v.z(), 0.0f,
                  n.x(), n.y(), n.z(), 0.0f,
                  0.0f,  0.0f,  0.0f,  1.0f );
    Matrix41 result(p.x(), p.y(), p.z(), 1.0f);
    result.multiply(m44); // result[] = p x m44[][]

    // PRINT THE VIEW MATRIX
    //if ( showMatrix ) {
    static bool once = false;
    if ( !once && p.x() > 0 && p.y() > 0 ) { once = true;
        Matrix41 m41(p.x(), p.y(), p.z(), 1.0f);
        printf("--- TO VIEWSPACE:\n"); 
        printf("   ");         p.show("  point"); printf("\n");
        printf("   "); _position.show("cam_pos"); printf("\n");
        printf("   ");   _lookAt.show(" lookat"); printf("\n");
        printf("   ");         u.show("  cam_u"); printf("\n");
        printf("   ");         v.show("  cam_v"); printf("\n");
        printf("   ");         n.show("  cam_n"); printf("\n");
        m44.show("uvm"); printf("\n");
        m41.show("point");
        result.show("out");
        printf("---\n");
    }
    p.xyz(result.x(), result.y(), result.z());      // return result in p
}
