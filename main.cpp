#include "plot.h"
#define _USE_MATH_DEFINES
#include <math.h>

int main()
{
	Plot p;
	p.xfunc = [](void*, unsigned int i) { return i * 2 * M_PI / 1000; };
	p.yfunc = [](void*, unsigned int i) { return sin(i * 2 * M_PI / 1000); };
	p.n = 1000;
	p.color = 0xFF0000;
	Plot::plot(1, p);
}