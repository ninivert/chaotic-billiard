#ifndef __PYBIND_CPP__
#define __PYBIND_CPP__

// https://github.com/pybind/pybind11/blob/smart_holder/README_smart_holder.rst
#include <pybind11/smart_holder.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "globals.h"
#include "vec2.hpp"
#include "ball.hpp"
#include "curve.hpp"
#include "world.hpp"
#include "collider.hpp"

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
	virtual vec2 ortho(double t) const override { PYBIND11_OVERRIDE_PURE(vec2, Curve, ortho, t); }
	virtual vec2 tangent(double t) const override { PYBIND11_OVERRIDE_PURE(vec2, Curve, tangent, t); }
	virtual std::string str() const override { PYBIND11_OVERRIDE_PURE(std::string, Curve, str); }
};

class PyLine : public Line, public py::trampoline_self_life_support {
public:
	using Line::Line;

	virtual vec2 operator()(double t, unsigned int order = 0) const override { PYBIND11_OVERRIDE(vec2, Line, operator(), t, order); }
	virtual vec2 ortho(double t) const override { PYBIND11_OVERRIDE(vec2, Line, ortho, t); }
	virtual vec2 tangent(double t) const override { PYBIND11_OVERRIDE(vec2, Line, tangent, t); }
	virtual std::string str() const override { PYBIND11_OVERRIDE(std::string, Line, str); }
};

class PySegment : public Segment, public py::trampoline_self_life_support {
public:
	using Segment::Segment;

	virtual vec2 operator()(double t, unsigned int order = 0) const override { PYBIND11_OVERRIDE(vec2, Segment, operator(), t, order); }
	virtual vec2 ortho(double t) const override { PYBIND11_OVERRIDE(vec2, Segment, ortho, t); }
	virtual vec2 tangent(double t) const override { PYBIND11_OVERRIDE(vec2, Segment, tangent, t); }
	virtual std::string str() const override { PYBIND11_OVERRIDE(std::string, Segment, str); }
};

class PyBezierCubic : public BezierCubic, public py::trampoline_self_life_support {
public:
	using BezierCubic::BezierCubic;

	virtual vec2 operator()(double t, unsigned int order = 0) const override { PYBIND11_OVERRIDE(vec2, BezierCubic, operator(), t, order); }
	virtual vec2 ortho(double t) const override { PYBIND11_OVERRIDE(vec2, BezierCubic, ortho, t); }
	virtual vec2 tangent(double t) const override { PYBIND11_OVERRIDE(vec2, BezierCubic, tangent, t); }
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
		.def("__call__", &Curve::operator());

	py::classh<Line, PyLine, Curve>(m, "Line")
		.def_readwrite("p", &Line::p)
		.def_readwrite("q", &Line::q)
		.def_readwrite("r", &Line::r)
		.def(py::init<>())
		.def(py::init<double, double, double>())
		.def(py::init<PyLine const&>())
		.def("ortho", &Line::ortho)
		.def("tangent", &Line::tangent)
		.def("__call__", &Line::operator());

	py::classh<Segment, PySegment, Curve>(m, "Segment")
		.def_readwrite("p1", &Segment::p1)
		.def_readwrite("p2", &Segment::p2)
		.def(py::init<>())
		.def(py::init<vec2 const&, vec2 const&>())
		.def(py::init<PySegment const&>())
		.def("ortho", &Segment::ortho)
		.def("tangent", &Segment::tangent)
		.def("__repr__", &Segment::str);

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
		.def("__repr__", &BezierCubic::str);

	py::class_<World>(m, "World")
		.def(py::init<>())
		.def("step", &World::step)
		.def("add_ball", &World::add_ball)
		.def("add_segment", &World::add_segment)
		.def_property_readonly("balls", [](World const& world) {
			return py::list(py::make_iterator(world.ball_ptrs.begin(), world.ball_ptrs.end()));
		})
		.def_property_readonly("segments", [](World const& world) {
			return py::list(py::make_iterator(world.segment_ptrs.begin(), world.segment_ptrs.end()));
		})
		.def("__repr__", &World::str);

	py::module_ m_globals = m.def_submodule("constants", "computational constants");
	m_globals.attr("eps") = EPS;  // TODO : make readonly

	py::module m_collider = m.def_submodule("collider", "compute collision points between mathematical parametrized paths");
	m_collider.def("line_line", &Collider::line_line);
	m_collider.def("segment_segment", &Collider::segment_segment);
	py::class_<Collider::ParamPair>(m_collider, "ParamPair")
		.def_readwrite("t1", &Collider::ParamPair::t1)
		.def_readwrite("t2", &Collider::ParamPair::t2);
}

#endif
