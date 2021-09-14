#include "collider.hpp"
#include "polynomial.hpp"  // Polynomial::roots_cubic
#include <algorithm>  // std::swap

Collider::ParamPairs Collider::line_line(Line const& l1, Line const& l2) {
	// Lines are infinite, and due to the tanh mapping,
	// these results maybe become quicky inaccurate.
	// Consider using Segments intead
	vec2 interpt(point_line_line(l1, l2));
	return { { l1.inverse(interpt), l2.inverse(interpt) } };
}

Collider::ParamPairs Collider::line_segment(Line const& line, Segment const& seg) {
	// Lines are infinite, and due to the tanh mapping,
	// these results maybe become quicky inaccurate.
	// Consider using Segments intead
	vec2 interpt(point_line_line(line, Line(seg)));
	return { { line.inverse(interpt), seg.inverse(interpt) } };
}

Collider::ParamPairs Collider::line_beziercubic(Line const& line, BezierCubic const& bezier) {
	std::vector<double> roots(params_line_beziercubic(line, bezier));
	Collider::ParamPairs tpairs;
	for (double root : roots) {
		vec2 interpt(bezier(root));
		tpairs.push_back({ line.inverse(interpt), root });
	}
	return tpairs;
}

Collider::ParamPairs Collider::segment_line(Segment const& seg, Line const& line) {
	Collider::ParamPairs tpairs(line_segment(line, seg));
	for (ParamPair& tpair : tpairs)
		std::swap(tpair.t1, tpair.t2);  // restitude order of segment t1 and line t2
	return tpairs;
}

Collider::ParamPairs Collider::segment_segment(Segment const& s1, Segment const& s2) {
	vec2 interpt(point_line_line(Line(s1), Line(s2)));
	return { { s1.inverse(interpt), s2.inverse(interpt) } };
}

Collider::ParamPairs Collider::segment_beziercubic(Segment const& seg, BezierCubic const& bezier) {
	std::vector<double> roots(params_line_beziercubic(Line(seg), bezier));
	Collider::ParamPairs tpairs;
	for (double root : roots) {
		vec2 interpt(bezier(root));
		tpairs.push_back({ seg.inverse(interpt), root });
	}
	return tpairs;
}

Collider::ParamPairs Collider::beziercubic_line(BezierCubic const& bezier, Line const& line) {
	Collider::ParamPairs tpairs(line_beziercubic(line, bezier));
	for (ParamPair& tpair : tpairs)
		std::swap(tpair.t1, tpair.t2);
	return tpairs;
}

Collider::ParamPairs Collider::beziercubic_segment(BezierCubic const& bezier, Segment const& seg) {
	Collider::ParamPairs tpairs(segment_beziercubic(seg, bezier));
	for (ParamPair& tpair : tpairs)
		std::swap(tpair.t1, tpair.t2);
	return tpairs;
}

Collider::ParamPairs Collider::beziercubic_beziercubic(BezierCubic const& b1, BezierCubic const& b2) {
	// TODO with a lookup table
	return { { 0, 0 } };
}

vec2 Collider::point_line_line(Line const& l1, Line const& l2) {
	double det = l1.p*l2.q - l2.p*l1.q;
	if (iszero(det))
		// lines are parallel
		// TODO : evaluate the plane quadrants (signs of infs) correctly
		return vec2(INFINITY, INFINITY);
	vec2 rhs = vec2(
		l1.r*l2.q - l2.r*l1.q,
		l1.p*l2.r - l2.p*l1.r
	);
	return rhs/det;
}

std::vector<double> Collider::params_line_beziercubic(Line const& line, BezierCubic const& bezier) {
	// construct cubic polynomial which solves the bezier-line intersection
	double p(line.p), q(line.q), r(line.r);
	double bx[4] = {
		-bezier.p0.x + 3*bezier.p1.x - 3*bezier.p2.x + bezier.p3.x,
		3*bezier.p0.x - 6*bezier.p1.x + 3*bezier.p2.x,
		-3*bezier.p0.x + 3*bezier.p1.x,
		bezier.p0.x
	};
	double by[4] = {
		-bezier.p0.y + 3*bezier.p1.y - 3*bezier.p2.y + bezier.p3.y,
		3*bezier.p0.y - 6*bezier.p1.y + 3*bezier.p2.y,
		-3*bezier.p0.y + 3*bezier.p1.y,
		bezier.p0.y
	};
	Polynomial::PolyCubic poly{
		p*bx[0] + q*by[0],
		p*bx[1] + q*by[1],
		p*bx[2] + q*by[2],
		p*bx[3] + q*by[3] - r
	};
	return Polynomial::roots_cubic(poly);
}
