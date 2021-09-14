#ifndef __GLOBALS_H__
#define __GLOBALS_H__

double constexpr EPS(1e-10);
unsigned int constexpr MAX_COLL_ITERS(10); // TODO : make this larger, currently small for ease of debugging

static bool iszero(double x, double eps = 1e-15) {
	return std::abs(x) < eps;
}

#endif
