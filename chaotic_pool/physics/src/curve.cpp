#include "curve.hpp"

//
// Line
//

Line::operator Segment() const {
	// mock-up segment included in line
	vec2 p1, p2;
	if (!iszero(q)) {
		// non-vertial line
		p1 = vec2(-1, (p - r) / q);
		p2 = vec2(1, -(p + r) / q);
	} else {
		// vertical line
		p1 = vec2(-r/p, -1);
		p2 = vec2(-r/p, 1);
	}
	return Segment(p1, p2);
}

vec2 Line::operator()(double t, unsigned int order /*= 0 */) const {
	Segment seg = Segment(*this);  // create a mock-up segment
	double s = std::atanh(2*t-1);  // map [0, 1] -> [-inf, inf]
	return (1-s)*seg.p1 + s*seg.p2;
}

double Line::inverse(vec2 const& point) const {
	Segment seg = Segment(*this);
	double s = seg.inverse(point);
	double t = (std::tanh(s)+1)/2;  // map [-inf, inf] -> [0, 1]
	return t;
}

Collider::ParamPairs Line::collide(Curve const& curve) const {
	Collider::ParamPairs tpairs = curve.collide(*this);
	for (Collider::ParamPair& tpair : tpairs)
		std::swap(tpair.t1, tpair.t2);
	return tpairs;
}
Collider::ParamPairs Line::collide(Line const& line) const { return Collider::line_line(*this, line); }
Collider::ParamPairs Line::collide(Segment const& seg) const { return Collider::line_segment(*this, seg); }
Collider::ParamPairs Line::collide(BezierCubic const& bezier) const { return Collider::line_beziercubic(*this, bezier); }

//
// Segment
//

Segment::operator Line() const {
	return Line(
		-(p2.y - p1.y),
		p2.x - p1.x,
		- p1.x*(p2.y - p1.y) + p1.y*(p2.x - p1.x)
	);
}

vec2 Segment::operator()(double t, unsigned int order /* = 0 */) const {
	return (1-t)*p1 + t*p2;
}

double Segment::inverse(vec2 const& point) const {
	// Given a point on the segment, return the t such that operator()(t) == point
	// Formally, t = (point - p1) / (p2 - p1), but we can't "divide" points
	// We check on what axis the biggest difference is, to get best precision
	double dx = std::abs(p1.x - p2.x);
	double dy = std::abs(p1.y - p2.y);
	if (iszero(dx) && iszero(dy))
		// p1 == p2 edge case
		return 0;
	if (dx > dy)
		return (point.x - p1.x) / (p2.x - p1.x);
	else
		return (point.y - p1.y) / (p2.y - p1.y);
}

Collider::ParamPairs Segment::collide(Curve const& curve) const {
	Collider::ParamPairs tpairs = curve.collide(*this);
	for (Collider::ParamPair& tpair : tpairs)
		std::swap(tpair.t1, tpair.t2);
	return tpairs;
}
Collider::ParamPairs Segment::collide(Line const& line) const { return Collider::segment_line(*this, line); }
Collider::ParamPairs Segment::collide(Segment const& seg) const { return Collider::segment_segment(*this, seg); }
Collider::ParamPairs Segment::collide(BezierCubic const& bezier) const { return Collider::segment_beziercubic(*this, bezier); }

//
// Bezier
//

vec2 BezierCubic::operator()(double t, unsigned int order /* = 0 */) const {
	assert(0 <= order && order <= 3);
	if (order == 0)
		return
			(1-t)*(1-t)*(1-t) * p0 +
			3*(1-t)*(1-t) * t * p1 +
			3*(1-t) * t*t * p2 +
			t*t*t * p3;
	if (order == 1)
		return
			3*(1-t)*(1-t) * (p1 - p0) +
			6*(1-t) * t * (p2 - p1) +
			3*t*t * (p3 - p2);
	if (order == 2)
		return
			6*(1-t)*(p2 - 2*p1 + p0) +
			6*t * (p3 - 2*p2 + p1);
	if (order == 3)
		return p0 - p1 - p2 + p3;
	// stop the compiler from complaining, we already have an assert to test other cases
	return vec2();
}

double BezierCubic::inverse(vec2 const& point) const {
	// TODO, implement with a precomputed lookup-table
	return 0;
}

Collider::ParamPairs BezierCubic::collide(Curve const& curve) const {
	Collider::ParamPairs tpairs = curve.collide(*this);
	for (Collider::ParamPair& tpair : tpairs)
		std::swap(tpair.t1, tpair.t2);
	return tpairs;
}
Collider::ParamPairs BezierCubic::collide(Line const& line) const { return Collider::beziercubic_line(*this, line); }
Collider::ParamPairs BezierCubic::collide(Segment const& seg) const { return Collider::beziercubic_segment(*this, seg); }
Collider::ParamPairs BezierCubic::collide(BezierCubic const& bezier) const { return Collider::beziercubic_beziercubic(*this, bezier); }
