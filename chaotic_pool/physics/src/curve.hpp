#ifndef __CURVE_HPP__
#define __CURVE_HPP__

#include <string>
#include "globals.h"

class Curve {
public:
	// Mathematically parametrized curve
	// Curve() = default;
	virtual ~Curve() = default;

	virtual vec2 operator()(double t, unsigned int order = 0) const = 0;
	virtual vec2 ortho(double t) const = 0;
	virtual vec2 tangent(double t) const = 0;
	virtual std::string str() const = 0;
};

class Line : public Curve {
public:
	double p, q, r;

	Line() = default;
	Line(double p_, double q_, double r_) : p(p_), q(q_), r(r_) {}
	Line(Line const& lc) : p(lc.p), q(lc.q), r(lc.r) {}

	// I would like to write this, but c++ won't allow me
	// virtual vec2 operator()(double t, unsigned int order = 0) const override = delete;
	virtual vec2 operator()(double t, unsigned int order = 0) const override {
		// TODO : do a atan mapping [0, 1] -> [-inf, inf]
		throw std::runtime_error("cannot parametrize an infinite line");
		return vec2();
	}
	virtual vec2 ortho(double t) const override { return vec2(p, q); }
	virtual vec2 tangent(double t) const override { return vec2(-q, p); }

	virtual std::string str() const override {
		return "Line(p=" + std::to_string(p) + ", q=" + std::to_string(q) + ", r=" + std::to_string(r) + ")";
	}
};

class Segment : public Curve {
public:
	vec2 p1, p2;

	Segment() = default;
	Segment(Segment const& l) = default;
	Segment(vec2 const& p1_, vec2 const& p2_) : p1(p1_), p2(p2_) {}
	virtual ~Segment() = default;

	operator Line() const {
		return Line(
			-(p2.y - p1.y),
			p2.x - p1.x,
			- p1.x*(p2.y - p1.y) + p1.y*(p2.x - p1.x)
		);
	}

	virtual vec2 operator()(double t, unsigned int order = 0) const override {
		return (1-t)*p1 + t*p2;
	}
	virtual vec2 ortho(double t = 0) const override { return tangent(t).ortho(); }
	virtual vec2 tangent(double t = 0) const override { return p1 - p2; }

	double inverse_param(vec2 const& point) const {
		// Given a point on the segment, return the t such that operator()(t) == point
		// Formally, t = (point - p1) / (p2 - p1), but we can't "divide" points
		// We check on what axis the biggest difference is, to get best precision
		double dx = std::abs(p1.x - p2.x);
		double dy = std::abs(p1.y - p2.y);
		if (dx > dy)
			return (point.x - p1.x) / (p2.x - p1.x);
		else
			return (point.y - p1.y) / (p2.y - p1.y);
	}

	bool in_bounds(vec2 const& p) const {
		// We take numerical errors into consideration, for instance a ball heading horizontally
		// collides with a vertical segment, then the collision point has to correspond exactly to the x coordinate of the segment
		// TODO : use a projector and compute the distance (here the corners have sqrt(2)*EPS more tolerance)
		return
			std::min(p1.x, p2.x) - EPS <= p.x && p.x <= std::max(p1.x, p2.x) + EPS &&
			std::min(p1.y, p2.y) - EPS <= p.y && p.y <= std::max(p1.y, p2.y) + EPS;
	}

	virtual std::string str() const override {
		return "Segment(p1=" + p1.str() + ", p2=" + p2.str() + ")";
	}
};

class BezierCubic : public Curve {
public:
	vec2 p0, p1, p2, p3;

	BezierCubic() = default;
	BezierCubic(BezierCubic const& l) = default;
	BezierCubic(vec2 const& p0_, vec2 const& p1_, vec2 const& p2_, vec2 const& p3_) : p0(p0_), p1(p1_), p2(p2_), p3(p3_) {}
	virtual ~BezierCubic() = default;

	virtual vec2 operator()(double t, unsigned int order = 0) const override {
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
	virtual vec2 ortho(double t = 0) const override { return tangent(t).ortho(); }
	virtual vec2 tangent(double t = 0) const override { return operator()(t, 1); }
};

#endif
