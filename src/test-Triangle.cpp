#include <stdio.h>
#include <vector>
#include "Triangle.h"
#include "Point3D.h"

using namespace std;

int main()
{
    Point3D p0( 10.00, 10.10, 10.20 );  // test 3 float ctor
    Point3D p1( 11.00, 11.10, 11.20 );
    Point3D p2( 12.00, 12.10, 12.20 );
    Point3D parr[3] = { p0, p1, p2 }; // test array of Point3D's

    //vector<Point3D> pvec;
    //pvec.push_back(parr[0]);
    //pvec.push_back(parr[1]);
    //pvec.push_back(parr[2]);

    Point3D pa(1000.00, 1000.10, 1000.20);
    Point3D pb(1001.00, 1001.10, 1001.20);
    Point3D pc(1002.00, 1002.10, 1002.20);

    Point3D pd(2000.00, 2000.10, 2000.20);
    Point3D pe(2001.00, 2001.10, 2001.20);
    Point3D pf(2002.00, 2002.10, 2002.20);

    // Test
    Triangle a(pa, pb, pc);   // ctor 3 points
    Triangle b(pd, pe, pf);   // ctor 3 points
    Triangle c(parr);         // ctor point array
    Triangle d(b);            // copy
    //Triangle e(pvec);         // copy vector

    Point3D tmp;
    tmp = pa;	// test Point3D assignment

    printf("EXPECT: a: { 1000.00, 1000.10, 1000.20 } { 1001.00, 1001.10, 1001.20 } { 1002.00, 1002.10, 1002.20 }\n");
    printf("   GOT: "); a.show("a"); printf("\n\n");

    printf("EXPECT: b: { 2000.00, 2000.10, 2000.20 } { 2001.00, 2001.10, 2001.20 } { 2002.00, 2002.10, 2002.20 }\n");
    printf("   GOT: "); b.show("b"); printf("\n\n");

    printf("EXPECT: c: { 10.00, 10.10, 10.20 } { 11.00, 11.10, 11.20 } { 12.00, 12.10, 12.20 }\n");
    printf("   GOT: "); c.show("c"); printf("\n\n");

    printf("EXPECT: d: { 2000.00, 2000.10, 2000.20 } { 2001.00, 2001.10, 2001.20 } { 2002.00, 2002.10, 2002.20 }\n");
    printf("   GOT: "); d.show("d"); printf("\n\n");

    //printf("EXPECT: e: { 10.00, 10.10, 10.20 } { 11.00, 11.10, 11.20 } { 12.00, 12.10, 12.20 }\n");
    //printf("   GOT: "); e.show("e"); printf("\n\n");

    printf("EXPECT: pa: { 1000.00, 1000.10, 1000.20 } tmp: { 1000.00, 1000.10, 1000.20 }\n");
    printf("   GOT: "); pa.show("pa"); printf(" "); tmp.show("tmp");

    return 0;
}
