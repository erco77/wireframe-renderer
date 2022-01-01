
// Create a cube of 'size' around model origin
DrawableObject* MakeCube(float size)
{
    //      0_ _ _1          Y+
    //     /|    /|          |
    //   3/_|_ 2/ |          |
    //   | 4|_ _|_|5         o______ X+
    //   | /    | /         /
    //   |/_ _ _|/         /
    //   7      6         Z+
    //
    DrawableObject *dobj = new DrawableObject();
    float s2 = size / 2;
    float v[8][3] = { { -s2, +s2, -s2 },   // 0
                      { +s2, +s2, -s2 },   // 1
                      { +s2, +s2, +s2 },   // 2
                      { -s2, +s2, +s2 },   // 3

                      { -s2, -s2, -s2 },   // 4
                      { +s2, -s2, -s2 },   // 5
                      { +s2, -s2, +s2 },   // 6
                      { -s2, -s2, +s2 }    // 7
                    };
    Point3D a,b,c,d;
    a = Point3D(v[0][0], v[0][1], v[0][2]); // 0-1-2-3 (top face)
    b = Point3D(v[1][0], v[1][1], v[1][2]);
    c = Point3D(v[2][0], v[2][1], v[2][2]);
    d = Point3D(v[3][0], v[3][1], v[3][2]);
    dobj->addQuad(new Quad(a,b,c,d));
    a = Point3D(v[4][0], v[4][1], v[4][2]); // 4-5-6-7 (bottom face)
    b = Point3D(v[5][0], v[5][1], v[5][2]);
    c = Point3D(v[6][0], v[6][1], v[6][2]);
    d = Point3D(v[7][0], v[7][1], v[7][2]);
    dobj->addQuad(new Quad(a,b,c,d));
    a = Point3D(v[2][0], v[2][1], v[2][2]); // 2-1-5-6 (right face)
    b = Point3D(v[1][0], v[1][1], v[1][2]);
    c = Point3D(v[5][0], v[5][1], v[5][2]);
    d = Point3D(v[6][0], v[6][1], v[6][2]);
    dobj->addQuad(new Quad(a,b,c,d));
    a = Point3D(v[3][0], v[3][1], v[3][2]); // 3-2-6-7 (front face)
    b = Point3D(v[2][0], v[2][1], v[2][2]);
    c = Point3D(v[6][0], v[6][1], v[6][2]);
    d = Point3D(v[7][0], v[7][1], v[7][2]);
    dobj->addQuad(new Quad(a,b,c,d));
    a = Point3D(v[0][0], v[0][1], v[0][2]); // 0-3-7-4 (left face)
    b = Point3D(v[3][0], v[3][1], v[3][2]);
    c = Point3D(v[7][0], v[7][1], v[7][2]);
    d = Point3D(v[4][0], v[4][1], v[4][2]);
    dobj->addQuad(new Quad(a,b,c,d));
    return dobj;
}
