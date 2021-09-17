#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <cmath>
#include <vector>

namespace Globals {
	extern double const EPS;
	extern unsigned int const MAX_COLL_ITERS; // TODO : make this larger, currently small for ease of debugging

	// floating point zero testing
	extern bool iszero(double x, double eps = 1e-15);
	// linear interpolation
	extern double lerp(double x, double y, double t);
	extern double ilerp(double x, double y, double s);
	extern double pfmod(double x, double y);
	extern std::vector<double> linspace(double start, double end, unsigned int n);
}


#endif
