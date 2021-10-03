#ifndef __BALL_HPP__
#define __BALL_HPP__

#include "vec2.hpp"
#include <string>  // std::string
#include <sstream>  // std::stringstream
#include <iostream>  // std::ostream

// TODO : do we need polymorphism for the Balls ?

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

static std::ostream& operator<<(std::ostream& stream, Ball const& ball) {
	return stream << ball.str() << std::endl;
}

#endif
