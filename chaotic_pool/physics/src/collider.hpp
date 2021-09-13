#ifndef __COLLIDER_HPP__
#define __COLLIDER_HPP__

#include "globals.h"
#include "vec2.hpp"
#include "curve.hpp"

namespace Collider {
	struct ParamPair { double t1, t2; };

	vec2 line_line(Line const& l1, Line const& l2) {
		double det = l1.p*l2.q - l2.p*l1.q;
		if (iszero(det))
			return vec2(INFINITY, INFINITY);  // lines are parallel
		vec2 rhs = vec2(
			l1.r*l2.q - l2.r*l1.q,
			l1.p*l2.r - l2.p*l1.r
		);
		return rhs/det;
	}

	ParamPair segment_segment(Segment const& s1, Segment const& s2) {
		vec2 interpt = line_line(s1, s2);  // implicit cast from segment to line
		return ParamPair{s1.inverse_param(interpt), s2.inverse_param(interpt)};
	}
}

#endif
