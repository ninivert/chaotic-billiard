#ifndef __SEGMENT_HPP__
#define __SEGMENT_HPP__

#include "vec2.hpp"
#include "globals.h"
#include <string>
#include <algorithm>

struct LineCoefs {
	double p, q, r;

	LineCoefs() = default;
	LineCoefs(double p_, double q_, double r_) : p(p_), q(q_), r(r_) {}
	LineCoefs(LineCoefs const& lc) : p(lc.p), q(lc.q), r(lc.r) {}

	std::string str() const {
		return "LineCoefs(p=" + std::to_string(p) + ", q=" + std::to_string(q) + ", r=" + std::to_string(r) + ")";
	}
};

struct SegmentIntersection {
	vec2 point;
	bool on_segment;

	SegmentIntersection() = default;
	SegmentIntersection(vec2 const& point_, bool on_segment_) : point(point_), on_segment(on_segment_) {}
	SegmentIntersection(SegmentIntersection const& s) : point(s.point), on_segment(s.on_segment) {}

	std::string str() const {
		return "SegmentIntersection(point=" + point.str() + ", on_segment=" + std::to_string(on_segment) + ")";
	}
};

struct Segment {
	vec2 p1, p2;

	Segment() = default;
	Segment(Segment const& l) = default;
	Segment(vec2 const& p1_, vec2 const& p2_) : p1(p1_), p2(p2_) {}
	virtual ~Segment() = default;

	LineCoefs coefs() const {
		return LineCoefs(
			-(p2.y - p1.y),
			p2.x - p1.x,
			- p1.x*(p2.y - p1.y) + p1.y*(p2.x - p1.x)
		);
	}

	vec2 ortho() const { return tangent().ortho(); }
	vec2 tangent() const { return p1 - p2; }

	bool in_bounds(vec2 const& p) const {
		// We take numerical errors into consideration, for instance a ball heading horizontally
		// collides with a vertical segment, then the collision point has to correspond exactly to the x coordinate of the segment
		// TODO : use a projector and compute the distance (here the corners have sqrt(2)*EPS more tolerance)
		return
			std::min(p1.x, p2.x) - EPS <= p.x && p.x <= std::max(p1.x, p2.x) + EPS &&
			std::min(p1.y, p2.y) - EPS <= p.y && p.y <= std::max(p1.y, p2.y) + EPS;
	}

	static vec2 intersect_linecoefs(LineCoefs const& c1, LineCoefs const& c2) {
		double det = c1.p*c2.q - c2.p*c1.q;  // is zero if the lines are parallel
		if (iszero(det))
			return vec2(INFINITY, INFINITY);
		vec2 rhs = vec2(
			c1.r*c2.q - c2.r*c1.q,
			c1.p*c2.r - c2.p*c1.r
		);
		return rhs/det;
	}

	// TODO : move this to intersector namespace
	static SegmentIntersection intersect(Segment const& s1, Segment const& s2) {
		vec2 point = intersect_linecoefs(s1.coefs(), s2.coefs());
		// TODO : use bool in_bounds
		bool on_segment =
			std::min(s1.p1.x, s1.p2.x) - EPS <= point.x && point.x <= std::max(s1.p1.x, s1.p2.x) + EPS &&
			std::min(s1.p1.y, s1.p2.y) - EPS <= point.y && point.y <= std::max(s1.p1.y, s1.p2.y) + EPS &&
			std::min(s2.p1.x, s2.p2.x) - EPS <= point.x && point.x <= std::max(s2.p1.x, s2.p2.x) + EPS &&
			std::min(s2.p1.y, s2.p2.y) - EPS <= point.y && point.y <= std::max(s2.p1.y, s2.p2.y) + EPS;
		return SegmentIntersection(point, on_segment);
	}

	virtual std::string str() const {
		return "Segment(p1=" + p1.str() + ", p2=" + p2.str() + ")";
	}
};


#endif
