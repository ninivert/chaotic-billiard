#ifndef __POLYNOMIAL_HPP__
#define __POLYNOMIAL_HPP__

#include <complex>
#include <cmath>
#include <vector>
#include <limits>
#include "globals.h"

// TODO : different EPS here ?

namespace Polynomial {
	double const nan(std::numeric_limits<double>::quiet_NaN());

	struct PolyCubic { double a, b, c, d; };
	typedef std::vector<double> PolyRoots;

	// https://github.com/ZhepeiWang/Root-Finder
	// *yoinks* thanks for the code, bro
	PolyRoots roots_cubic(PolyCubic const& poly) {
		// solve ax³ + bx² + cx + d = 0
		PolyRoots roots;
		double a(poly.a), b(poly.b), c(poly.c), d(poly.d);

		constexpr double cos120 = -0.50;
		constexpr double sin120 = 0.866025403784438646764;

		if (Globals::iszero(d)) {
			// First solution is x = 0
			roots.push_back(0.0);

			// Converting to a quadratic equation
			d = c;
			c = b;
			b = a;
			a = 0.0;
		}

		if (Globals::iszero(a)) {
			if (Globals::iszero(b)) {
				// Linear equation
				if (!Globals::iszero(c))
					roots.push_back(-d / c);
			}
			else {
				// Quadratic equation
				double discriminant = c * c - 4.0 * b * d;
				if (discriminant >= 0) {
					double inv2b = 1.0 / (2.0 * b);
					double y = std::sqrt(discriminant);
					roots.push_back((-c + y) * inv2b);
					roots.push_back((-c - y) * inv2b);
				}
			}
		}
		else {
			// Cubic equation
			double inva = 1.0 / a;
			double invaa = inva * inva;
			double bb = b * b;
			double bover3a = b * (1.0 / 3.0) * inva;
			double p = (3.0 * a * c - bb) * (1.0 / 3.0) * invaa;
			double halfq = (2.0 * bb * b - 9.0 * a * b * c + 27.0 * a * a * d) * (0.5 / 27.0) * invaa * inva;
			double yy = p * p * p / 27.0 + halfq * halfq;

			if (!Globals::iszero(yy) && yy > 0) {
				// Sqrt is positive: one real solution
				double y = std::sqrt(yy);
				double uuu = -halfq + y;
				double vvv = -halfq - y;
				double www = std::fabs(uuu) > std::fabs(vvv) ? uuu : vvv;
				double w = (www < 0) ? -std::pow(std::fabs(www), 1.0 / 3.0) : std::pow(www, 1.0 / 3.0);
				roots.push_back(w - p / (3.0 * w) - bover3a);
			}
			else if (!Globals::iszero(yy) && yy < 0) {
				// Sqrt is negative: three real solutions
				double x = -halfq;
				double y = std::sqrt(-yy);
				double theta;
				double r;
				double ux;
				double uyi;
				// Convert to polar form
				if (std::fabs(x) > Globals::EPS) {
					theta = (x > 0.0) ? std::atan(y / x) : (std::atan(y / x) + M_PI);
					r = std::sqrt(x * x - yy);
				}
				else {
					// Vertical line
					theta = M_PI / 2.0;
					r = y;
				}
				// Calculate cube root
				theta /= 3.0;
				r = std::pow(r, 1.0 / 3.0);
				// Convert to complex coordinate
				ux = std::cos(theta) * r;
				uyi = std::sin(theta) * r;
				// First solution
				roots.push_back(ux + ux - bover3a);
				// Second solution, rotate +120 degrees
				roots.push_back(2.0 * (ux * cos120 - uyi * sin120) - bover3a);
				// Third solution, rotate -120 degrees
				roots.push_back(2.0 * (ux * cos120 + uyi * sin120) - bover3a);
			}
			else {
				// Sqrt is zero: two real solutions
				double www = -halfq;
				double w = (www < 0.0) ? -std::pow(std::fabs(www), 1.0 / 3.0) : std::pow(www, 1.0 / 3.0);
				// First solution
				roots.push_back(w + w - bover3a);
				// Second solution, rotate +120 degrees
				roots.push_back(2.0 * w * cos120 - bover3a);
			}
		}
		return roots;
	}
}

#endif
