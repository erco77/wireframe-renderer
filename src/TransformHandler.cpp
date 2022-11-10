// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#include <math.h>
#include "TransformHandler.h"
#include "Camera.h"
#include "Matrix41.h"
#include "Matrix44.h"

void TransformHandler::rotateX(Point3D& p,  // point to be rotated
                               float angle) // angle in radians
{
    float cosD = cosf(angle);
    float sinD = sinf(angle);
    Matrix44 m44( 1.0f, 0.0f,  0.0f, 0.0f,
                  0.0f, cosD, -sinD, 0.0f,
                  0.0f, sinD,  cosD, 0.0f,
                  0.0f, 0.0f,  0.0f, 1.0f );
    Matrix41 result(p.x(), p.y(), p.z(), 1.0f);
    result.multiply(m44);  // result = p x [ m44 ]
    p.xyz(result.x(), result.y(), result.z());      // return result in p
}

void TransformHandler::rotateX(Triangle& t,  // triangle to be rotated
                               float angle)  // angle in radians
{
    rotateX(t.a(), angle);
    rotateX(t.b(), angle);
    rotateX(t.c(), angle);
}

void TransformHandler::rotateX(Quad& q,      // quad to be rotated
                               float angle)  // angle in radians
{
    rotateX(q.a(), angle);
    rotateX(q.b(), angle);
    rotateX(q.c(), angle);
    rotateX(q.d(), angle);
}

void TransformHandler::rotateObjectX(DrawableObject *dobj, float angle)
{
    unsigned int i;
    for (i = 0; i < dobj->triangles()->size(); i++) {
        rotateX(*dobj->triangle(i), angle);
    }
    for (i = 0; i < dobj->quads()->size(); i++) {
        rotateX(*dobj->quad(i), angle);
    }
}

void TransformHandler::rotateY(Point3D& p, float angle)
{
    float cosD = cosf(angle);
    float sinD = sinf(angle);
    Matrix44 m44( cosD,  0.0f, sinD, 0.0f,
                  0.0f,  1.0f, 0.0f, 0.0f,
                  -sinD, 0.0f, cosD, 0.0f,
                  0.0f,  0.0f, 0.0f, 1.0f );
    Matrix41 result(p.x(), p.y(), p.z(), 1.0f);

    p.x(result.x());
    p.y(result.y());
    p.z(result.z());
}

void TransformHandler::rotateObjectY(DrawableObject *dobj, float angle)
{
    unsigned int i;
    for (i = 0; i < dobj->triangles()->size(); i++) {
        Triangle *t = dobj->triangle(i);
        // TODO: rotateX(t, angle)
        rotateY(t->a(), angle);
        rotateY(t->b(), angle);
        rotateY(t->c(), angle);
    }
    for (i = 0; i < dobj->quads()->size(); i++) {
        Quad *q = dobj->quad(i);
        // TODO: rotateX(q, angle)
        rotateY(q->a(), angle);
        rotateY(q->b(), angle);
        rotateY(q->c(), angle);
        rotateY(q->d(), angle);
    }
}

void TransformHandler::rotateZ(Point3D& p, float angle)
{
    float cosD = cosf(angle);
    float sinD = sinf(angle);
    Matrix44 m44( cosD, -sinD, 0.0f, 0.0f,
                  sinD,  cosD, 0.0f, 0.0f,
                  0.0f,  0.0f, 1.0f, 0.0f,
                  0.0f,  0.0f, 0.0f, 1.0f );
    Matrix41 result(p.x(), p.y(), p.z(), 1.0f);
    p.xyz(result.x(), result.y(), result.z());      // return result in p
}

void TransformHandler::rotateObjectZ(DrawableObject *dobj, float angle)
{
    unsigned int i;
    for (i = 0; i < dobj->triangles()->size(); i++) {
        Triangle *t = dobj->triangle(i);
        // TODO: rotateX(t, angle)
        rotateZ(t->a(), angle);
        rotateZ(t->b(), angle);
        rotateZ(t->c(), angle);
    }
    for (i = 0; i < dobj->quads()->size(); i++) {
        Quad *q = dobj->quad(i);
        // TODO: rotateX(q, angle)
        rotateZ(q->a(), angle);
        rotateZ(q->b(), angle);
        rotateZ(q->c(), angle);
        rotateZ(q->d(), angle);
    }
}

void TransformHandler::scaleUniform(Point3D& p, float factor)
{
    Matrix44 m44( factor,   0.0f,   0.0f, 0.0f,
                    0.0f, factor,   0.0f, 0.0f,
                    0.0f,   0.0f, factor, 0.0f,
                    0.0f,   0.0f,   0.0f, 1.0f );
    Matrix41 result(p.x(), p.y(), p.z(), 1.0f);
    result.multiply(m44);                           // result = p x [ m44 ]
    p.xyz(result.x(), result.y(), result.z());      // return result in p
}

void TransformHandler::scaleObjectUniform(DrawableObject *dobj, float factor)
{
    unsigned int i;
    for ( i=0; i<dobj->triangles()->size(); i++) {
        Triangle *t = dobj->triangle(i);
        scaleUniform(t->a(), factor);
        scaleUniform(t->b(), factor);
        scaleUniform(t->c(), factor);
    }
    for ( i=0; i<dobj->quads()->size(); i++) {
        Quad *q = dobj->quad(i);
        scaleUniform(q->a(), factor);
        scaleUniform(q->b(), factor);
        scaleUniform(q->c(), factor);
        scaleUniform(q->d(), factor);
    }
}

// Handle NDC Transformation (Normalized Device Space)
//     Projection from eye coords to clip space
//
void TransformHandler::toNDC(Point3D& p,
                             float near, float far,
                             float height, float width)
{
    float nw = (2 * near) / width;
    float nh = (2 * near) / height;
    float f1 = (  -(far + near)) / (far - near);
    float f2 = (-2*(far * near)) / (far - near);

    Matrix44 m44(   nw, 0.0f,  0.0f, 0.0f,
                  0.0f,   nh,  0.0f, 0.0f,
                  0.0f, 0.0f,    f1,   f2,
                  0.0f, 0.0f, -1.0f, 0.0f );
    Matrix41 result(p.x(), p.y(), p.z(), 1.0f);
    result.multiply(m44);  // result = p x [ m44 ]

    // Make sure to normalize with w
    p.x( result.x() / result.w() );
    p.y( result.y() / result.w() );
    p.z( result.z() / result.w() );
}

// Normalized Device Space
void TransformHandler::objectToNDC(DrawableObject *dobj,
                                   const Camera& camera,
                                   float scrn_w,
                                   float scrn_h)
{
    float near   = camera.getNear();
    float far    = camera.getFar();
    float angle  = camera.getHorizontalViewAngle() * M_PI/180;
    float width  = (tan(angle / 2) * near) * 2;
    float height = width / (scrn_w / scrn_h);

    unsigned int i;
    for ( i=0; i<dobj->triangles()->size(); i++) {
        Triangle *t = dobj->triangle(i);
        toNDC(t->a(), near, far, height, width);
        toNDC(t->b(), near, far, height, width);
        toNDC(t->c(), near, far, height, width);
    }
    for ( i=0; i<dobj->quads()->size(); i++) {
        Quad *q = dobj->quad(i);
        toNDC(q->a(), near, far, height, width);
        toNDC(q->b(), near, far, height, width);
        toNDC(q->c(), near, far, height, width);
        toNDC(q->d(), near, far, height, width);
    }
}

void TransformHandler::objectToScreenSpace(DrawableObject *dobj, float scrn_w)
{
    float scrw2 = (scrn_w / 2.0);
    unsigned int j;
    for (j = 0; j < dobj->triangles()->size(); j++) {
        Triangle *t = dobj->triangle(j);

        t->a().x(t->a().x() * scrw2 + scrw2);
        t->a().y(t->a().y() * scrw2 + scrw2);

        t->b().x(t->b().x() * scrw2 + scrw2);
        t->b().y(t->b().y() * scrw2 + scrw2);

        t->c().x(t->c().x() * scrw2 + scrw2);
        t->c().y(t->c().y() * scrw2 + scrw2);
    }
    for (j = 0; j < dobj->quads()->size(); j++) {
        Quad *t = dobj->quad(j);

        t->a().x(t->a().x() * scrw2 + scrw2);
        t->a().y(t->a().y() * scrw2 + scrw2);

        t->b().x(t->b().x() * scrw2 + scrw2);
        t->b().y(t->b().y() * scrw2 + scrw2);

        t->c().x(t->c().x() * scrw2 + scrw2);
        t->c().y(t->c().y() * scrw2 + scrw2);

        t->d().x(t->d().x() * scrw2 + scrw2);
        t->d().y(t->d().y() * scrw2 + scrw2);
    }
}

void TransformHandler::showCamera(const Camera& camera)
{
    const bool showMatrix = true;
    Point3D p(0.0f, 0.0f, 0.0f);
    camera.pointToViewSpace(p, showMatrix);
}

