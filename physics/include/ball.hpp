#ifndef __BALL_HPP__
#define __BALL_HPP__

#include "vec2.hpp"
#include <string>
#include <sstream>
#include <limits>

class Ball {
public:
	vec2 pos, pos_prev, vel;

	Ball() = default;
	Ball(Ball const&) = default;
	Ball(vec2 const& pos_, vec2 const& vel_) : pos(pos_), pos_prev(pos_), vel(vel_) {}
	virtual ~Ball() = default;

	virtual std::string str() const {
		return "Ball(pos=" + pos.str() + ", vel=" + vel.str() + ")";
	}

	virtual std::string json() const {
		std::stringstream ss;
		ss
			<< "{"
				<< "\"class\":" << "\"Ball\"" << ","
				<< "\"parameters\":"
				<< "{"
					<< "\"pos\":" << pos.json() << ","
					<< "\"vel\":" << vel.json()
				<< "}"
			<< "}";
		return ss.str();
	}
};

// TODO : ostream methods

#endif
