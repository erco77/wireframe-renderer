// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#pragma once
#include <string>
#include <vector>
#include "Triangle.h"
#include "Quad.h"
#include "Camera.h"

using namespace std;

typedef void (LineFunc)(int x1,int y1,int x2,int y2);

// Container to handle one or more primitives as a single class
class DrawableObject {
private:
    vector<Triangle*> *_triangles;
    vector<Quad*> *_quads;
public:
    DrawableObject();
    DrawableObject(const Point3D& p);
    DrawableObject(float x, float y, float z);
    DrawableObject(const DrawableObject& o);
    DrawableObject(const Point3D& position, vector<Triangle*> *triangles);
    DrawableObject(const Point3D& position, vector<Quad*> *_quads);
    ~DrawableObject(void);

    // Triangles
    void addTriangle(Triangle* t) { _triangles->push_back(t); }
    const vector<Triangle*>* triangles() { return _triangles; }
    Triangle* triangle(int i) { return _triangles->at(i); }

    // Quads
    void addQuad(Quad *q) { _quads->push_back(q); }
    const vector<Quad*> *quads() { return _quads; }
    Quad* quad(int i) { return _quads->at(i); }

    // Ways to transform the drawable object's objects
    void translate(const float& x, const float& y, const float& z);
    void toViewSpace(const Camera& camera);
    void objectToNDC(const Camera& camera, float scrn_w, float scrn_h);

    bool isBehindCamera(const Camera& cam);
    void draw(LineFunc *drawLine);
};
