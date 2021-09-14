#ifndef __CURVE_HPP__
#define __CURVE_HPP__

#include <string>
#include <cmath>
#include <cassert>  // assert
#include <algorithm>  // std::swap
#include <vector>  // std::vector
#include "globals.h"
#include "vec2.hpp"
#include "collider.hpp"

// forward declarations
class Curve;
class Line;
class Segment;
class BezierCubic;
namespace Collider {
	struct ParamPair;
	typedef std::vector<ParamPair> ParamPairs;
}

class Curve {
public:
	// smooth curve defined by operator()(t) for all t in [0, 1]
	virtual ~Curve() = default;

	virtual vec2 operator()(double t, unsigned int order = 0) const = 0;
	virtual double inverse(vec2 const& point) const = 0;
	virtual vec2 ortho(double t) const = 0;
	virtual vec2 tangent(double t) const = 0;

	virtual Collider::ParamPairs collide(Curve const& curve) const = 0;
	virtual Collider::ParamPairs collide(Line const& line) const = 0;
	virtual Collider::ParamPairs collide(Segment const& seg) const = 0;
	virtual Collider::ParamPairs collide(BezierCubic const& bezier) const = 0;

	virtual std::string str() const = 0;
};

class Line : public Curve {
public:
	// all vec2(x, y) such that px + qy + r = 0
	double p, q, r;

	Line() = default;
	Line(double p_, double q_, double r_) : p(p_), q(q_), r(r_) {}
	Line(Line const& lc) : p(lc.p), q(lc.q), r(lc.r) {}

	explicit operator Segment() const;

	virtual vec2 operator()(double t, unsigned int order = 0) const override;
	virtual double inverse(vec2 const& point) const override;
	virtual vec2 ortho(double t) const override { return vec2(p, q); }
	virtual vec2 tangent(double t) const override { return vec2(-q, p); }

	virtual Collider::ParamPairs collide(Curve const& curve) const override;
	virtual Collider::ParamPairs collide(Line const& line) const override;
	virtual Collider::ParamPairs collide(Segment const& seg) const override;
	virtual Collider::ParamPairs collide(BezierCubic const& bezier) const override;

	virtual std::string str() const override {
		return "Line(p=" + std::to_string(p) + ", q=" + std::to_string(q) + ", r=" + std::to_string(r) + ")";
	}
};

class Segment : public Curve {
public:
	// all points such that there exists t in [0, 1] st (1-t)*p1 + t*p2 = point
	vec2 p1, p2;

	Segment() = default;
	Segment(Segment const& l) = default;
	Segment(vec2 const& p1_, vec2 const& p2_) : p1(p1_), p2(p2_) {}
	virtual ~Segment() = default;

	explicit operator Line() const;

	virtual vec2 operator()(double t, unsigned int order = 0) const override;
	virtual double inverse(vec2 const& point) const override;
	virtual vec2 ortho(double t = 0) const override { return tangent(t).ortho(); }
	virtual vec2 tangent(double t = 0) const override { return p1 - p2; }

	virtual Collider::ParamPairs collide(Curve const& curve) const override;
	virtual Collider::ParamPairs collide(Line const& line) const override;
	virtual Collider::ParamPairs collide(Segment const& seg) const override;
	virtual Collider::ParamPairs collide(BezierCubic const& bezier) const override;

	virtual std::string str() const override {
		return "Segment(p1=" + p1.str() + ", p2=" + p2.str() + ")";
	}
};

class BezierCubic : public Curve {
public:
	// cubic bezier curve with control points p0, p1, p2, p3
	vec2 p0, p1, p2, p3;

	BezierCubic() = default;
	BezierCubic(BezierCubic const& l) = default;
	BezierCubic(vec2 const& p0_, vec2 const& p1_, vec2 const& p2_, vec2 const& p3_) : p0(p0_), p1(p1_), p2(p2_), p3(p3_) {}
	virtual ~BezierCubic() = default;

	virtual vec2 operator()(double t, unsigned int order = 0) const override;
	virtual double inverse(vec2 const& point) const override;
	virtual vec2 ortho(double t = 0) const override { return tangent(t).ortho(); }
	virtual vec2 tangent(double t = 0) const override { return operator()(t, 1); }

	virtual Collider::ParamPairs collide(Curve const& curve) const override;
	virtual Collider::ParamPairs collide(Line const& line) const override;
	virtual Collider::ParamPairs collide(Segment const& seg) const override;
	virtual Collider::ParamPairs collide(BezierCubic const& bezier) const override;

	virtual std::string str() const override {
		return "BezierCubic(p0=" + p0.str() + ", p1=" + p1.str() + ", p2=" + p2.str() + ", p3=" + p3.str() + ")";
	}
};

#endif
