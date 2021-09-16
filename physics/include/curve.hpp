#ifndef __CURVE_HPP__
#define __CURVE_HPP__

#include <string>  // std::string
#include <sstream>  // std::stringstream
#include <limits>  // std::numeric_limits
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
class Arc;
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
	virtual Collider::ParamPairs collide(Arc const& arc) const = 0;
	virtual Collider::ParamPairs collide(BezierCubic const& bezier) const = 0;

	virtual std::string str() const = 0;
	virtual std::string json() const = 0;
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
	virtual vec2 ortho(double t) const override;
	virtual vec2 tangent(double t) const override;

	virtual Collider::ParamPairs collide(Curve const& curve) const override;
	virtual Collider::ParamPairs collide(Line const& line) const override;
	virtual Collider::ParamPairs collide(Segment const& seg) const override;
	virtual Collider::ParamPairs collide(Arc const& arc) const override;
	virtual Collider::ParamPairs collide(BezierCubic const& bezier) const override;

	virtual std::string str() const override {
		return "Line(p=" + std::to_string(p) + ", q=" + std::to_string(q) + ", r=" + std::to_string(r) + ")";
	}
	virtual std::string json() const override {
		std::stringstream ss;
		ss.precision(std::numeric_limits<double>::digits10);
		ss
			<< "{"
				<< "\"class\":" << "\"Line\"" << ","
				<< "\"parameters\":"
				<< "{"
					<< "\"p\":" << p << ","
					<< "\"q\":" << q << ","
					<< "\"r\":" << r
				<< "}"
			<< "}";
		return ss.str();
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
	virtual vec2 ortho(double t) const override;
	virtual vec2 tangent(double t) const override;

	virtual Collider::ParamPairs collide(Curve const& curve) const override;
	virtual Collider::ParamPairs collide(Line const& line) const override;
	virtual Collider::ParamPairs collide(Segment const& seg) const override;
	virtual Collider::ParamPairs collide(Arc const& arc) const override;
	virtual Collider::ParamPairs collide(BezierCubic const& bezier) const override;

	virtual std::string str() const override {
		return "Segment(p1=" + p1.str() + ", p2=" + p2.str() + ")";
	}
	virtual std::string json() const override {
		std::stringstream ss;
		ss.precision(std::numeric_limits<double>::digits10);
		ss
			<< "{"
				<< "\"class\":" << "\"Segment\"" << ","
				<< "\"parameters\":"
				<< "{"
					<< "\"p1\":" << p1.json() << ","
					<< "\"p2\":" << p2.json()
				<< "}"
			<< "}";
		return ss.str();
	}
};

class Arc : public Curve {
public:
	vec2 p0;
	double r;
	double theta_min, theta_max;

	Arc() = default;
	Arc(Arc const& a) = default;
	Arc(vec2 const& p0_, double r_, double theta_min_, double theta_max_)
		: p0(p0_), r(r_),
		theta_min(pfmod(theta_min_, 2*M_PI)),
		// we add the extra term to force the arc to go around anticlockwise
		theta_max(pfmod(theta_max_, 2*M_PI) + (pfmod(theta_max_, 2*M_PI) <= pfmod(theta_min_, 2*M_PI))*2*M_PI)
		{}
	virtual ~Arc() = default;

	virtual vec2 operator()(double t, unsigned int order = 0) const override;
	virtual double inverse(vec2 const& point) const override;
	virtual vec2 ortho(double t) const override;
	virtual vec2 tangent(double t) const override;

	virtual Collider::ParamPairs collide(Curve const& curve) const override;
	virtual Collider::ParamPairs collide(Line const& line) const override;
	virtual Collider::ParamPairs collide(Segment const& seg) const override;
	virtual Collider::ParamPairs collide(Arc const& arc) const override;
	virtual Collider::ParamPairs collide(BezierCubic const& bezier) const override;

	virtual std::string str() const override {
		return "Arc(p0=" + p0.str() + ", r=" + std::to_string(r) + ", theta_min=" + std::to_string(theta_min) + ", theta_max=" + std::to_string(theta_max) + ")";
	}
	virtual std::string json() const override {
		std::stringstream ss;
		ss.precision(std::numeric_limits<double>::digits10);
		ss
			<< "{"
				<< "\"class\":" << "\"Arc\"" << ","
				<< "\"parameters\":"
				<< "{"
					<< "\"p0\":" << p0.json() << ","
					<< "\"r\":" << r << ","
					<< "\"theta_min\":" << theta_min << ","
					<< "\"theta_max\":" << theta_max
				<< "}"
			<< "}";
		return ss.str();
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
	virtual vec2 ortho(double t) const override;
	virtual vec2 tangent(double t) const override;

	virtual Collider::ParamPairs collide(Curve const& curve) const override;
	virtual Collider::ParamPairs collide(Line const& line) const override;
	virtual Collider::ParamPairs collide(Segment const& seg) const override;
	virtual Collider::ParamPairs collide(Arc const& arc) const override;
	virtual Collider::ParamPairs collide(BezierCubic const& bezier) const override;

	virtual std::string str() const override {
		return "BezierCubic(p0=" + p0.str() + ", p1=" + p1.str() + ", p2=" + p2.str() + ", p3=" + p3.str() + ")";
	}
	virtual std::string json() const override {
		std::stringstream ss;
		ss.precision(std::numeric_limits<double>::digits10);
		ss
			<< "{"
				<< "\"class\":" << "\"BezierCubic\"" << ","
				<< "\"parameters\":"
				<< "{"
					<< "\"p0\":" << p0.json() << ","
					<< "\"p1\":" << p1.json() << ","
					<< "\"p2\":" << p2.json() << ","
					<< "\"p3\":" << p3.json()
				<< "}"
			<< "}";
		return ss.str();
	}
};

#endif
