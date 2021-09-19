#ifndef __FROM_JSON_HPP__
#define __FROM_JSON_HPP__

#include <cassert>  // assert
#include <memory> // std::make_shared
#include <iostream>

#include "world.hpp"
#include "ball.hpp"
#include "curve.hpp"
#include "vec2.hpp"

#include "json.hpp"

// Utility functions to parse json
// These methods are not in the physics classes,
// because I don't want to bloat the includes in the physics module

vec2 vec2_from_json(nlohmann::json const& j) {
	assert(j["class"] == "vec2");
	return vec2(j["parameters"]["x"], j["parameters"]["y"]);
}

Ball Ball_from_json(nlohmann::json const& j) {
	assert(j["class"] == "Ball");
	return Ball(vec2_from_json(j["parameters"]["pos"]), vec2_from_json(j["parameters"]["vel"]));
}

Segment Segment_from_json(nlohmann::json const& j) {
	assert(j["class"] == "Segment");
	return Segment(vec2_from_json(j["parameters"]["p1"]), vec2_from_json(j["parameters"]["p2"]));
}

Arc Arc_from_json(nlohmann::json const& j) {
	assert(j["class"] == "Arc");
	return Arc(vec2_from_json(j["parameters"]["p0"]), j["parameters"]["r"], j["parameters"]["theta_min"], j["parameters"]["theta_max"]);
}

BezierCubic BezierCubic_from_json(nlohmann::json const& j) {
	assert(j["class"] == "BezierCubic");
	return BezierCubic(vec2_from_json(j["parameters"]["p0"]), vec2_from_json(j["parameters"]["p1"]), vec2_from_json(j["parameters"]["p2"]), vec2_from_json(j["parameters"]["p3"]));
}

World World_from_json(nlohmann::json const& j) {
	World world;

	for (auto& el : j["balls"].items())
		world.add_ball(std::make_shared<Ball>(Ball_from_json(el.value())));

	for (auto& el : j["curves"].items()) {
		if (el.value()["class"] == "Segment") {
			world.add_curve(std::make_shared<Segment>(Segment_from_json(el.value())));
		}
		else if (el.value()["class"] == "Arc") {
			world.add_curve(std::make_shared<Arc>(Arc_from_json(el.value())));
		}
		else if (el.value()["class"] == "BezierCubic") {
			world.add_curve(std::make_shared<BezierCubic>(BezierCubic_from_json(el.value())));
		}
		else {
			std::cout << "class `" << el.value()["class"] << "` not recognized" << std::endl;
		}
	}

	return world;
}

#endif
