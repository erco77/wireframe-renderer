// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#pragma once
#include <vector>
#include "Point3D.h"

class Camera {
private:
    Point3D _position;
    Point3D _lookAt;
    float   _near;
    float   _far;
    float   _horizontalViewAngle;

    // Precomputed values
    //     These are updated by Recalc() and saved
    //     to prevent repeated computations during rendering..
    //
    Point3D precomp_new_y_axis;
    Point3D precomp_int_ortho_axis;
    Point3D precomp_view_plane_normal;
    Point3D precomp_up_vector;
    float   precomp_horizontalViewAngleRadians;
    float   precomp_NDCWidth;;

protected:
    void Recalc();

public:
    Camera();
    Camera(const Point3D& position,
           const Point3D& lookAt,
           float horizontalViewAngle,
           float near, float far);
    ~Camera(void);

    void setPosition(const Point3D& position)   { _position  = position;          Recalc(); }
    void setPosition(float x, float y, float z) { _position.xyz(x,y,z);           Recalc(); }
    void setLookAt(const Point3D& point)        { _lookAt = point;                Recalc(); }
    void setLookAt(float x, float y, float z)   { _lookAt.xyz(x,y,z);             Recalc(); }
    void setNear(float z)                       { _near = z;                      Recalc(); }
    void setFar(float z)                        { _far  = z;                      Recalc(); }
    void setHorizontalViewAngle(float degrees)  { _horizontalViewAngle = degrees; Recalc(); }

    void xyz(float x, float y, float z)         { _position.xyz(x,y,z);           Recalc(); }
    void xyzLookAt(float x, float y, float z)   { _lookAt.xyz(x,y,z);             Recalc(); }
    void xyzLookAtTranslate(const float& x, const float& y, const float& z) { _lookAt.translate(x,y,z); Recalc(); }

    const Point3D& getPosition()            const { return _position; }
    const Point3D& getLookAt()              const { return _lookAt; }
    float getNear()                         const { return _near; }
    float getFar()                          const { return _far; }
    float getHorizontalViewAngle()          const { return _horizontalViewAngle; }
    float getHorizontalViewAngleRadians()   const { return precomp_horizontalViewAngleRadians; }

    Point3D getIntermediateOrthogonalAxis() const { return precomp_int_ortho_axis; }
    Point3D getViewPlaneNormal()            const { return precomp_view_plane_normal; }
    Point3D getNewYAxis()                   const { return precomp_new_y_axis; }
    Point3D getUpVector()                   const { return precomp_up_vector; } 

    // New accessors
    float x()       const { return _position.x(); }
    float y()       const { return _position.y(); }
    float z()       const { return _position.z(); }
//  float xLookAt() const { return _lookAt.x(); }
//  float yLookAt() const { return _lookAt.y(); }
//  float zLookAt() const { return _lookAt.z(); }
    const Point3D& lookAt() const { return _lookAt; }

    float getNDCWidth() const { return precomp_NDCWidth; }

    void pointToViewSpace(Point3D& p, bool showMatrix=false) const;

    // Ways to transform camera
    void translate(const float& x, const float& y, const float& z) { _position.translate(x,y,z); Recalc(); }
};
