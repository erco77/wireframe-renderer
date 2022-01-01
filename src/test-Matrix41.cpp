#include "Matrix41.h"

int main()
{
    {
	float f41[4][1] = { { 1.0 }, { 2.0 }, { 3.0 }, { 4.0 } };
	Matrix41 m41(f41);
	m41.show("f4-1dig");
    }

    {
	float f41[4][1] = { { 1.0 }, { 222.0 }, { 3333.0 }, { 44.0 } };
	Matrix41 m41(f41);
	m41.show("f4-3dig");
    }

    {
	float f41[4][1] = { { -11111.0 }, { 2.0 }, { 33.0 }, { 44000.0 } };
	Matrix41 m41(f41);
	m41.show("f4-3dig");
    }
}
