#include "physics/collider.hpp"
#include "physics/polynomial.hpp"  // Polynomial::roots_cubic
#include "physics/logger.hpp"
#include <algorithm>  // std::swap

using namespace Globals;

//
// Line - ...
//

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

Collider::ParamPairs Collider::line_arc(Line const& line, Arc const& arc) {
	std::vector<vec2> interpts(points_segment_arc(Segment(line), arc));
	Collider::ParamPairs tpairs;
	for (vec2 const& interpt : interpts)
		tpairs.push_back({ line.inverse(interpt), arc.inverse(interpt) });
	return tpairs;
}

Collider::ParamPairs Collider::line_ellipse(Line const& line, Ellipse const& ellipse) {
	// TODO
	return { { 0, 0 } };
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

//
// Segment - ...
//

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

Collider::ParamPairs Collider::segment_arc(Segment const& seg, Arc const& arc) {
	std::vector<vec2> interpts(points_segment_arc(seg, arc));
	Collider::ParamPairs tpairs;
	for (vec2 const& interpt : interpts)
		tpairs.push_back({ seg.inverse(interpt), arc.inverse(interpt) });
	return tpairs;
}

Collider::ParamPairs Collider::segment_ellipse(Segment const& seg, Ellipse const& ellipse) {
	// TODO
	return { {0,0} };
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

//
// Arc - ...
//

Collider::ParamPairs Collider::arc_line(Arc const& arc, Line const& line) {
	Collider::ParamPairs tpairs(line_arc(line, arc));
	for (ParamPair& tpair : tpairs)
		std::swap(tpair.t1, tpair.t2);  // restitude order of segment t1 and line t2
	return tpairs;
}

Collider::ParamPairs Collider::arc_segment(Arc const& arc, Segment const& seg) {
	Collider::ParamPairs tpairs(segment_arc(seg, arc));
	for (ParamPair& tpair : tpairs)
		std::swap(tpair.t1, tpair.t2);  // restitude order of segment t1 and line t2
	return tpairs;
}

Collider::ParamPairs Collider::arc_arc(Arc const& arc1, Arc const& arc2) {
	// TODO
	return { {0,0} };
}

Collider::ParamPairs Collider::arc_ellipse(Arc const& arc, Ellipse const& ellipse) {
	// TODO
	return { {0,0} };
}

Collider::ParamPairs Collider::arc_beziercubic(Arc const& arc, BezierCubic const& bezier) {
	// TODO
	return { { 0, 0 } };
}

//
// Ellipse - ...
//


Collider::ParamPairs Collider::ellipse_line(Ellipse const& ellipse, Line const& line) {
	// TODO
	return {{0,0}};
}
Collider::ParamPairs Collider::ellipse_segment(Ellipse const& ellipse, Segment const& seg) {
	// TODO
	return {{0,0}};
}
Collider::ParamPairs Collider::ellipse_arc(Ellipse const& ellipse, Arc const& arc2) {
	// TODO
	return {{0,0}};
}
Collider::ParamPairs Collider::ellipse_ellipse(Ellipse const& ellipse1, Ellipse const& ellipse2) {
	// TODO
	return {{0,0}};
}
Collider::ParamPairs Collider::ellipse_beziercubic(Ellipse const& ellipse, BezierCubic const& bezier) {
	// TODO
	return {{0,0}};
}


//
// BezierCubic - ...
//

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

Collider::ParamPairs Collider::beziercubic_arc(BezierCubic const& bezier, Arc const& arc) {
	// TODO
	return { {0,0} };
}

Collider::ParamPairs Collider::beziercubic_ellipse(BezierCubic const& bezier, Ellipse const& ellipse) {
	// TODO
	return { {0,0} };
}

Collider::ParamPairs Collider::beziercubic_beziercubic(BezierCubic const& b1, BezierCubic const& b2) {
	// TODO with a lookup table
	return { { 0, 0 } };
}

//
// Utility
//

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

std::vector<vec2> Collider::points_segment_arc(Segment const& seg, Arc const& arc) {
	// https://mathworld.wolfram.com/Circle-LineIntersection.html
	vec2 relp1(seg.p1 - arc.p0);
	vec2 relp2(seg.p2 - arc.p0);
	double dx(relp2.x - relp1.x);
	double dy(relp2.y - relp1.y);
	double dr(std::sqrt(dx*dx + dy*dy));
	double det(relp1.x*relp2.y - relp2.x*relp1.y);
	double delta(arc.r*arc.r * dr*dr - det*det);
	if (delta <= 0)
		// no collision
		return {};
	if (iszero(delta)) {
		// one collision point
		vec2 relinterpt(det*dy/(dr*dr), -det*dx/(dr*dr));
		return { relinterpt + arc.p0 };
	} else {
		// two collision points
		double sqrtdelta(std::sqrt(delta));
		vec2 relinterpt1(
			(det*dy + std::copysign(dx, dy)*sqrtdelta) / (dr*dr),
			(-det*dx + std::copysign(dy, dx)*sqrtdelta) / (dr*dr)
		);
		vec2 relinterpt2(
			(det*dy - std::copysign(dx, dy)*sqrtdelta) / (dr*dr),
			(-det*dx - std::copysign(dy, dx)*sqrtdelta) / (dr*dr)
		);
		// Logger::debug(relinterpt1.str() + " " + relinterpt2.str());
		return { relinterpt1 + arc.p0, relinterpt2 + arc.p0 };
	}
}

// http://ambrsoft.com/TrigoCalc/Circles2/Ellipse/EllipseLine.htm
// segment-ellipse

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
