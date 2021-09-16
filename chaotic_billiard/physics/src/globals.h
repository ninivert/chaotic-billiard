#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <cmath>
#include <vector>

double constexpr EPS(1e-10);
unsigned int constexpr MAX_COLL_ITERS(10); // TODO : make this larger, currently small for ease of debugging

// TODO : make this inline and/or extern ?
// floating point precision zero testing
static bool iszero(double x, double eps = 1e-15) { return std::abs(x) < eps;}
// linear interpolation
static double lerp(double x, double y, double t) { return x + (y-x)*t; }
// inverse linear interpolation st ilerp(a, b, lerp(a, b, t)) == t
static double ilerp(double x, double y, double s) { return (s-x)/(y-x); }
// Positive fmod st x in [0, y], instead of x in [-y, y]
static double pfmod(double x, double y) { return std::fmod(x, y) + (x < 0)*std::abs(y); }
// linspace
static std::vector<double> linspace(double start, double end, unsigned int n) {
	if (n == 0)
		return {};

	if (n == 1)
		return { start };

	std::vector<double> ret(n);
	double delta((end - start) / (n - 1));

	for (unsigned int i(0); i < (n-1); ++i)
		ret[i] = (start + delta * i);
	ret[n-1] = end;  // ensure last element is strictly equal to end

	return ret;
}


#endif
