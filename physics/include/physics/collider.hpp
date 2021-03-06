#ifndef __COLLIDER_HPP__
#define __COLLIDER_HPP__

#include <string>
#include <vector>
#include "globals.h"
#include "curve.hpp"

// forward declarations
class Line;
class Segment;
class Arc;
class Ellipse;
class BezierCubic;

namespace Collider {
	struct ParamPair {
		double t1, t2;
		bool on_first() const { return 0 - Globals::EPS <= t1 && t1 <= 1 + Globals::EPS; }
		bool on_second() const { return 0 - Globals::EPS <= t2 && t2 <= 1 + Globals::EPS; }
		bool on_both() const { return on_first() && on_second(); }
		std::string str() const { return "ParamPair(t1=" + std::to_string(t1) + ", t2=" + std::to_string(t2) + ")" ; }
	};
	typedef std::vector<ParamPair> ParamPairs;

	ParamPairs line_line(Line const& l1, Line const& l2);
	ParamPairs line_segment(Line const& line, Segment const& seg);
	ParamPairs line_arc(Line const& line, Arc const& arc);
	ParamPairs line_ellipse(Line const& line, Ellipse const& ellipse);
	ParamPairs line_beziercubic(Line const& line, BezierCubic const& bezier);

	ParamPairs segment_line(Segment const& seg, Line const& line);
	ParamPairs segment_segment(Segment const& s1, Segment const& s2);
	ParamPairs segment_arc(Segment const& seg, Arc const& arc);
	ParamPairs segment_ellipse(Segment const& seg, Ellipse const& ellipse);
	ParamPairs segment_beziercubic(Segment const& seg, BezierCubic const& bezier);

	ParamPairs arc_line(Arc const& arc, Line const& line);
	ParamPairs arc_segment(Arc const& arc, Segment const& seg);
	ParamPairs arc_arc(Arc const& arc1, Arc const& arc2);
	ParamPairs arc_ellipse(Arc const& arc, Ellipse const& ellipse);
	ParamPairs arc_beziercubic(Arc const& arc, BezierCubic const& bezier);

	ParamPairs ellipse_line(Ellipse const& ellipse, Line const& line);
	ParamPairs ellipse_segment(Ellipse const& ellipse, Segment const& seg);
	ParamPairs ellipse_arc(Ellipse const& ellipse, Arc const& arc2);
	ParamPairs ellipse_ellipse(Ellipse const& ellipse1, Ellipse const& ellipse2);
	ParamPairs ellipse_beziercubic(Ellipse const& ellipse, BezierCubic const& bezier);

	ParamPairs beziercubic_line(BezierCubic const& bezier, Line const& line);
	ParamPairs beziercubic_segment(BezierCubic const& bezier, Segment const& seg);
	ParamPairs beziercubic_arc(BezierCubic const& bezier, Arc const& arc);
	ParamPairs beziercubic_ellipse(BezierCubic const& bezier, Ellipse const& ellipse);
	ParamPairs beziercubic_beziercubic(BezierCubic const& b1, BezierCubic const& b2);

	vec2 point_line_line(Line const& l1, Line const& l2);
	std::vector<vec2> points_segment_arc(Segment const& seg, Arc const& arc);
	std::vector<double> params_line_beziercubic(Line const& line, BezierCubic const& bezier);
}

#endif
