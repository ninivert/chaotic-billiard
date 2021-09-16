#ifndef __PYBIND_CPP__
#define __PYBIND_CPP__

// https://github.com/pybind/pybind11/blob/smart_holder/README_smart_holder.rst
#include <pybind11/smart_holder.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "globals.h"
#include "logger.hpp"
#include "vec2.hpp"
#include "ball.hpp"
#include "curve.hpp"
#include "world.hpp"

namespace py = pybind11;

// Trampoline classes
// https://github.com/pybind/pybind11/blob/smart_holder/README_smart_holder.rst#trampolines-and-stdunique_ptr

class PyBall : public Ball, public py::trampoline_self_life_support {
public:
	using Ball::Ball;

	virtual std::string str() const override {
		PYBIND11_OVERRIDE(std::string, Ball, str);
	}
};

class PyCurve : public Curve, public py::trampoline_self_life_support {
public:
	using Curve::Curve;

	virtual vec2 operator()(double t, unsigned int order = 0) const override { PYBIND11_OVERRIDE_PURE(vec2, Curve, operator(), t, order); }
	virtual double inverse(vec2 const& point) const override { PYBIND11_OVERRIDE_PURE(double, Curve, inverse, point); }
	virtual vec2 ortho(double t) const override { PYBIND11_OVERRIDE_PURE(vec2, Curve, ortho, t); }
	virtual vec2 tangent(double t) const override { PYBIND11_OVERRIDE_PURE(vec2, Curve, tangent, t); }

	virtual Collider::ParamPairs collide(Line const& line) const override { PYBIND11_OVERRIDE_PURE(Collider::ParamPairs, Curve, collide, line); }
	virtual Collider::ParamPairs collide(Segment const& seg) const override { PYBIND11_OVERRIDE_PURE(Collider::ParamPairs, Curve, collide, seg); }
	virtual Collider::ParamPairs collide(Arc const& arc) const override { PYBIND11_OVERRIDE_PURE(Collider::ParamPairs, Curve, collide, arc); }
	virtual Collider::ParamPairs collide(BezierCubic const& bezier) const override { PYBIND11_OVERRIDE_PURE(Collider::ParamPairs, Curve, collide, bezier); }

	virtual std::string str() const override { PYBIND11_OVERRIDE_PURE(std::string, Curve, str); }
};

class PyLine : public Line, public py::trampoline_self_life_support {
public:
	using Line::Line;

	virtual vec2 operator()(double t, unsigned int order = 0) const override { PYBIND11_OVERRIDE(vec2, Line, operator(), t, order); }
	virtual double inverse(vec2 const& point) const override { PYBIND11_OVERRIDE(double, Line, inverse, point); }
	virtual vec2 ortho(double t) const override { PYBIND11_OVERRIDE(vec2, Line, ortho, t); }
	virtual vec2 tangent(double t) const override { PYBIND11_OVERRIDE(vec2, Line, tangent, t); }

	virtual Collider::ParamPairs collide(Line const& line) const override { PYBIND11_OVERRIDE(Collider::ParamPairs, Line, collide, line); }
	virtual Collider::ParamPairs collide(Segment const& seg) const override { PYBIND11_OVERRIDE(Collider::ParamPairs, Line, collide, seg); }
	virtual Collider::ParamPairs collide(Arc const& arc) const override { PYBIND11_OVERRIDE(Collider::ParamPairs, Line, collide, arc); }
	virtual Collider::ParamPairs collide(BezierCubic const& bezier) const override { PYBIND11_OVERRIDE(Collider::ParamPairs, Line, collide, bezier); }

	virtual std::string str() const override { PYBIND11_OVERRIDE(std::string, Line, str); }
};

class PySegment : public Segment, public py::trampoline_self_life_support {
public:
	using Segment::Segment;

	virtual vec2 operator()(double t, unsigned int order = 0) const override { PYBIND11_OVERRIDE(vec2, Segment, operator(), t, order); }
	virtual double inverse(vec2 const& point) const override { PYBIND11_OVERRIDE(double, Segment, inverse, point); }
	virtual vec2 ortho(double t) const override { PYBIND11_OVERRIDE(vec2, Segment, ortho, t); }
	virtual vec2 tangent(double t) const override { PYBIND11_OVERRIDE(vec2, Segment, tangent, t); }

	virtual Collider::ParamPairs collide(Line const& line) const override { PYBIND11_OVERRIDE(Collider::ParamPairs, Segment, collide, line); }
	virtual Collider::ParamPairs collide(Segment const& seg) const override { PYBIND11_OVERRIDE(Collider::ParamPairs, Segment, collide, seg); }
	virtual Collider::ParamPairs collide(Arc const& arc) const override { PYBIND11_OVERRIDE(Collider::ParamPairs, Segment, collide, arc); }
	virtual Collider::ParamPairs collide(BezierCubic const& bezier) const override { PYBIND11_OVERRIDE(Collider::ParamPairs, Segment, collide, bezier); }

	virtual std::string str() const override { PYBIND11_OVERRIDE(std::string, Segment, str); }
};

class PyArc : public Arc, public py::trampoline_self_life_support {
public:
	using Arc::Arc;

	virtual vec2 operator()(double t, unsigned int order = 0) const override { PYBIND11_OVERRIDE(vec2, Arc, operator(), t, order); }
	virtual double inverse(vec2 const& point) const override { PYBIND11_OVERRIDE(double, Arc, inverse, point); }
	virtual vec2 ortho(double t) const override { PYBIND11_OVERRIDE(vec2, Arc, ortho, t); }
	virtual vec2 tangent(double t) const override { PYBIND11_OVERRIDE(vec2, Arc, tangent, t); }

	virtual Collider::ParamPairs collide(Line const& line) const override { PYBIND11_OVERRIDE(Collider::ParamPairs, Arc, collide, line); }
	virtual Collider::ParamPairs collide(Segment const& seg) const override { PYBIND11_OVERRIDE(Collider::ParamPairs, Arc, collide, seg); }
	virtual Collider::ParamPairs collide(Arc const& arc) const override { PYBIND11_OVERRIDE(Collider::ParamPairs, Arc, collide, arc); }
	virtual Collider::ParamPairs collide(BezierCubic const& bezier) const override { PYBIND11_OVERRIDE(Collider::ParamPairs, Arc, collide, bezier); }

	virtual std::string str() const override { PYBIND11_OVERRIDE(std::string, Arc, str); }
};

class PyBezierCubic : public BezierCubic, public py::trampoline_self_life_support {
public:
	using BezierCubic::BezierCubic;

	virtual vec2 operator()(double t, unsigned int order = 0) const override { PYBIND11_OVERRIDE(vec2, BezierCubic, operator(), t, order); }
	virtual double inverse(vec2 const& point) const override { PYBIND11_OVERRIDE(double, BezierCubic, inverse, point); }
	virtual vec2 ortho(double t) const override { PYBIND11_OVERRIDE(vec2, BezierCubic, ortho, t); }
	virtual vec2 tangent(double t) const override { PYBIND11_OVERRIDE(vec2, BezierCubic, tangent, t); }

	virtual Collider::ParamPairs collide(Line const& line) const override { PYBIND11_OVERRIDE(Collider::ParamPairs, BezierCubic, collide, line); }
	virtual Collider::ParamPairs collide(Segment const& seg) const override { PYBIND11_OVERRIDE(Collider::ParamPairs, BezierCubic, collide, seg); }
	virtual Collider::ParamPairs collide(Arc const& arc) const override { PYBIND11_OVERRIDE(Collider::ParamPairs, BezierCubic, collide, arc); }
	virtual Collider::ParamPairs collide(BezierCubic const& bezier) const override { PYBIND11_OVERRIDE(Collider::ParamPairs, BezierCubic, collide, bezier); }

	virtual std::string str() const override { PYBIND11_OVERRIDE(std::string, BezierCubic, str); }
};

// Publicist for binding protected member functions
// https://pybind11.readthedocs.io/en/stable/advanced/classes.html#binding-protected-member-functions

// class WorldPublicist : public World {
// public:
// 	using World::ball_ptrs;
// 	using World::segment_ptrs;
// };

// Smart holder
// https://github.com/pybind/pybind11/blob/smart_holder/README_smart_holder.rst#trampolines-and-stdunique_ptr

PYBIND11_SMART_HOLDER_TYPE_CASTERS(Curve)
PYBIND11_SMART_HOLDER_TYPE_CASTERS(Line)
PYBIND11_SMART_HOLDER_TYPE_CASTERS(Segment)
PYBIND11_SMART_HOLDER_TYPE_CASTERS(Arc)
PYBIND11_SMART_HOLDER_TYPE_CASTERS(BezierCubic)
PYBIND11_SMART_HOLDER_TYPE_CASTERS(Ball)

// Binding code

PYBIND11_MODULE(physics, m) {
	py::class_<vec2>(m, "vec2")
		.def_readwrite("x", &vec2::x)
		.def_readwrite("y", &vec2::y)
		.def(py::init<double, double>(), py::arg("x") = 0.0, py::arg("y") = 0.0)
		.def(py::init<vec2>())
		.def(py::init<>())
		.def(py::self == py::self)
		.def(py::self != py::self)
		.def(py::self + py::self)
		.def(py::self - py::self)
		.def(- py::self)
		.def(py::self += py::self)
		.def(py::self -= py::self)
		.def(py::self + double())
		.def(py::self - double())
		.def(py::self * double())
		.def(py::self / double())
		.def(double() + py::self)
		.def(double() - py::self)
		.def(double() * py::self)
		.def(double() / py::self)
		.def(py::self += double())
		.def(py::self -= double())
		.def(py::self *= double())
		.def(py::self /= double())
		.def("length", &vec2::length)
		.def("rotate", &vec2::rotate)
		.def("normalize", &vec2::normalize)
		.def("ortho", &vec2::ortho)
		.def_static("dist", &vec2::dist)
		.def_static("dot", &vec2::dot)
		.def_static("cross", &vec2::cross)
		.def("__getitem__", [](vec2 const& v, unsigned int index) {
			if (index == 0) return v.x;
			if (index == 1) return v.y;
			throw std::out_of_range("out of bounds index `" + std::to_string(index) + "` on vec2 instance");
		})
		.def("__repr__", &vec2::str);

	py::classh<Ball, PyBall>(m, "Ball")
		.def_readwrite("pos", &Ball::pos)
		.def_readwrite("pos_prev", &Ball::pos_prev)
		.def_readwrite("vel", &Ball::vel)
		.def(py::init<>())
		.def(py::init<vec2 const&, vec2 const&>())
		.def(py::init<PyBall const&>())
		.def("__repr__", &Ball::str);

	py::classh<Curve, PyCurve>(m, "Curve")
		.def("ortho", &Curve::ortho)
		.def("tangent", &Curve::tangent)
		.def("collide", static_cast<Collider::ParamPairs (Curve::*)(Line const&) const>(&Curve::collide))
		.def("collide", static_cast<Collider::ParamPairs (Curve::*)(Segment const&) const>(&Curve::collide))
		.def("collide", static_cast<Collider::ParamPairs (Curve::*)(Arc const&) const>(&Curve::collide))
		.def("collide", static_cast<Collider::ParamPairs (Curve::*)(BezierCubic const&) const>(&Curve::collide))
		.def("inverse", &Curve::inverse)
		.def("__call__", &Curve::operator(), py::arg("t"), py::arg("order") = 0)
		.def("__repr__", &Curve::str);

	py::classh<Line, PyLine, Curve>(m, "Line")
		.def_readwrite("p", &Line::p)
		.def_readwrite("q", &Line::q)
		.def_readwrite("r", &Line::r)
		.def(py::init<>())
		.def(py::init<double, double, double>())
		.def(py::init<PyLine const&>())
		.def("ortho", &Line::ortho)
		.def("tangent", &Line::tangent)
		.def("collide", static_cast<Collider::ParamPairs (Line::*)(Line const&) const>(&Line::collide))
		.def("collide", static_cast<Collider::ParamPairs (Line::*)(Segment const&) const>(&Line::collide))
		.def("collide", static_cast<Collider::ParamPairs (Line::*)(Arc const&) const>(&Line::collide))
		.def("collide", static_cast<Collider::ParamPairs (Line::*)(BezierCubic const&) const>(&Line::collide))
		.def("inverse", &Line::inverse)
		.def("__call__", &Line::operator(), py::arg("t"), py::arg("order") = 0)
		.def("__repr__", &Line::str);

	py::classh<Segment, PySegment, Curve>(m, "Segment")
		.def_readwrite("p1", &Segment::p1)
		.def_readwrite("p2", &Segment::p2)
		.def(py::init<>())
		.def(py::init<vec2 const&, vec2 const&>())
		.def(py::init<PySegment const&>())
		.def("ortho", &Segment::ortho)
		.def("tangent", &Segment::tangent)
		.def("collide", static_cast<Collider::ParamPairs (Segment::*)(Line const&) const>(&Segment::collide))
		.def("collide", static_cast<Collider::ParamPairs (Segment::*)(Segment const&) const>(&Segment::collide))
		.def("collide", static_cast<Collider::ParamPairs (Segment::*)(Arc const&) const>(&Segment::collide))
		.def("collide", static_cast<Collider::ParamPairs (Segment::*)(BezierCubic const&) const>(&Segment::collide))
		.def("inverse", &Segment::inverse)
		.def("__call__", &Segment::operator(), py::arg("t"), py::arg("order") = 0)
		.def("__repr__", &Segment::str);

	py::classh<Arc, PyArc, Curve>(m, "Arc")
		.def_readwrite("p0", &Arc::p0)
		.def_readwrite("r", &Arc::r)
		.def_readwrite("theta_min", &Arc::theta_min)
		.def_readwrite("theta_max", &Arc::theta_max)
		.def(py::init<>())
		.def(py::init<vec2 const&, double, double, double>())
		.def(py::init<PyArc const&>())
		.def("ortho", &Arc::ortho)
		.def("tangent", &Arc::tangent)
		.def("collide", static_cast<Collider::ParamPairs (Arc::*)(Line const&) const>(&Arc::collide))
		.def("collide", static_cast<Collider::ParamPairs (Arc::*)(Segment const&) const>(&Arc::collide))
		.def("collide", static_cast<Collider::ParamPairs (Arc::*)(Arc const&) const>(&Arc::collide))
		.def("collide", static_cast<Collider::ParamPairs (Arc::*)(BezierCubic const&) const>(&Arc::collide))
		.def("inverse", &Arc::inverse)
		.def("__call__", &Arc::operator(), py::arg("t"), py::arg("order") = 0)
		.def("__repr__", &Arc::str);

	py::classh<BezierCubic, PyBezierCubic, Curve>(m, "BezierCubic")
		.def_readwrite("p1", &BezierCubic::p0)
		.def_readwrite("p1", &BezierCubic::p1)
		.def_readwrite("p2", &BezierCubic::p2)
		.def_readwrite("p1", &BezierCubic::p3)
		.def(py::init<>())
		.def(py::init<vec2 const&, vec2 const&, vec2 const&, vec2 const&>())
		.def(py::init<PyBezierCubic const&>())
		.def("ortho", &BezierCubic::ortho)
		.def("tangent", &BezierCubic::tangent)
		.def("collide", static_cast<Collider::ParamPairs (BezierCubic::*)(Line const&) const>(&BezierCubic::collide))
		.def("collide", static_cast<Collider::ParamPairs (BezierCubic::*)(Segment const&) const>(&BezierCubic::collide))
		.def("collide", static_cast<Collider::ParamPairs (BezierCubic::*)(Arc const&) const>(&BezierCubic::collide))
		.def("collide", static_cast<Collider::ParamPairs (BezierCubic::*)(BezierCubic const&) const>(&BezierCubic::collide))
		.def("inverse", &BezierCubic::inverse)
		.def("__call__", &BezierCubic::operator(), py::arg("t"), py::arg("order") = 0)
		.def("__repr__", &BezierCubic::str);

	py::class_<World>(m, "World")
		.def(py::init<>())
		.def("step", &World::step)
		.def("add_ball", &World::add_ball)
		.def("add_curve", &World::add_curve)
		.def_property_readonly("balls", [](World const& world) {
			return py::list(py::make_iterator(world.ball_ptrs.begin(), world.ball_ptrs.end()));
		})
		.def_property_readonly("curves", [](World const& world) {
			return py::list(py::make_iterator(world.curve_ptrs.begin(), world.curve_ptrs.end()));
		})
		.def("__repr__", &World::str);

	py::module_ m_collider = m.def_submodule("collider", "collider utility functions");
	py::class_<Collider::ParamPair>(m_collider, "ParamPair")
		.def_readwrite("t1", &Collider::ParamPair::t1)
		.def_readwrite("t2", &Collider::ParamPair::t2)
		.def("on_first", &Collider::ParamPair::on_first)
		.def("on_second", &Collider::ParamPair::on_second)
		.def("on_both", &Collider::ParamPair::on_both)
		.def("__repr__", &Collider::ParamPair::str);

	py::module_ m_globals = m.def_submodule("constants", "computational constants");
	m_globals.attr("eps") = EPS;  // TODO : make readonly

	py::module_ m_logger = m.def_submodule("logger", "logger to stdout used internally");
	m_logger.attr("level") = Logger::level;  // TODO : make this work when changed in python
	py::enum_<Logger::Level>(m_logger, "Level")
		.value("DEBUG", Logger::DEBUG)
		.value("INFO", Logger::INFO)
		.value("WARNING", Logger::WARNING)
		.value("ERROR", Logger::ERROR)
		.value("CRITICAL", Logger::CRITICAL)
		.export_values();
}

#endif
