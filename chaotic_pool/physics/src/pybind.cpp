#ifndef __PYBIND_CPP__
#define __PYBIND_CPP__

// https://github.com/pybind/pybind11/blob/smart_holder/README_smart_holder.rst
#include <pybind11/smart_holder.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "globals.h"
#include "vec2.hpp"
#include "ball.hpp"
#include "segment.hpp"
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

class PySegment : public Segment, public py::trampoline_self_life_support {
public:
	using Segment::Segment;

	virtual std::string str() const override {
		PYBIND11_OVERRIDE(std::string, Segment, str);
	}
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

PYBIND11_SMART_HOLDER_TYPE_CASTERS(Ball)
PYBIND11_SMART_HOLDER_TYPE_CASTERS(Segment)

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

	py::class_<LineCoefs>(m, "LineCoefs")
		.def_readwrite("p", &LineCoefs::p)
		.def_readwrite("q", &LineCoefs::q)
		.def_readwrite("r", &LineCoefs::r)
		.def("__repr__", &LineCoefs::str);

	py::class_<SegmentIntersection>(m, "SegmentIntersection")
		.def_readwrite("point", &SegmentIntersection::point)
		.def_readwrite("on_segment", &SegmentIntersection::on_segment)
		.def("__repr__", &SegmentIntersection::str);

	py::classh<Segment, PySegment>(m, "Segment")
		.def_readwrite("p1", &Segment::p1)
		.def_readwrite("p2", &Segment::p2)
		.def(py::init<>())
		.def(py::init<vec2 const&, vec2 const&>())
		.def(py::init<PySegment const&>())
		.def("coefs", &Segment::coefs)
		.def("ortho", &Segment::ortho)
		.def("tangent", &Segment::tangent)
		.def("in_bounds", &Segment::in_bounds)
		.def_static("intersect_linecoefs", &Segment::intersect_linecoefs)
		.def_static("intersect", &Segment::intersect)
		.def("__repr__", &Segment::str);

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
}

#endif
