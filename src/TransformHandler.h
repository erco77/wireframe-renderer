// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#pragma once
#include "Point3D.h"
#include "Camera.h"
#include "DrawableObject.h"

class TransformHandler {
public:
    TransformHandler() { }
    ~TransformHandler() { }

    // Note: angles are radians
    static void rotateX(Point3D& p, float angle);
    static void rotateX(Triangle& t, float angle);
    static void rotateX(Quad& q, float angle);
    static void rotateObjectX(DrawableObject *dobj, float angle);
    static void rotateY(Point3D& p, float angle);
    static void rotateObjectY(DrawableObject *dobj, float angle);
    static void rotateZ(Point3D& p, float angle);
    static void rotateObjectZ(DrawableObject *dobj, float angle);
    static void scaleUniform(Point3D &p, float factor);
    static void scaleObjectUniform(DrawableObject *dobj, float factor);
    static void toNDC(Point3D &p, float near, float far, float height, float width);
    static void objectToNDC(DrawableObject *dobj, const Camera& camera, float scrn_w, float scrn_h);
    static void objectToViewSpace(DrawableObject *dobj, const Camera& camera);
    static void objectToScreenSpace(DrawableObject *dobj, float scrn_w);
    static void showCamera(const Camera& camera);
};

