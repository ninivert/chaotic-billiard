#ifndef __VEC2_HPP__
#define __VEC2_HPP__

#include <cmath>
#include <string>
#include <sstream>
#include <iostream>

struct vec2 {
	double x, y;

	vec2() : x(0), y(0) {}
	vec2(double x, double y) : x(x), y(y) {}
	vec2(const vec2& v) = default;
	vec2(vec2&& v) = default;

	vec2& operator=(const vec2& v) {
		x = v.x;
		y = v.y;
		return *this;
	}

	bool operator==(const vec2& other) { return other.x == x && other.y == y; }
	bool operator!=(const vec2& other) { return !(*this == other); }
	friend bool operator==(vec2 const& v1, vec2 const& v2) { return v1 == v2; }
	friend bool operator!=(vec2 const& v1, vec2 const& v2) { return v1 != v2; }

	vec2 operator+(vec2 const& v) const { return vec2(x + v.x, y + v.y); }
	vec2 operator-(vec2 const& v) const { return vec2(x - v.x, y - v.y); }
	vec2 operator-() const { return vec2(-x, -y); }

	vec2& operator+=(vec2 const& v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	vec2& operator-=(vec2 const& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}

	vec2 operator+(double s) const { return vec2(x + s, y + s); }
	vec2 operator-(double s) const { return vec2(x - s, y - s); }
	vec2 operator*(double s) const { return vec2(x * s, y * s); }
	vec2 operator/(double s) const { return vec2(x / s, y / s); }
	friend vec2 operator+(double s, vec2 const& v) { return vec2(s + v.x, s + v.y); }
	friend vec2 operator-(double s, vec2 const& v) { return vec2(s - v.x, s - v.y); }
	friend vec2 operator*(double s, vec2 const& v) { return vec2(s * v.x, s * v.y); }
	friend vec2 operator/(double s, vec2 const& v) { return vec2(s / v.x, s / v.y); }

	vec2& operator+=(double s) {
		x += s;
		y += s;
		return *this;
	}
	vec2& operator-=(double s) {
		x -= s;
		y -= s;
		return *this;
	}
	vec2& operator*=(double s) {
		x *= s;
		y *= s;
		return *this;
	}
	vec2& operator/=(double s) {
		x /= s;
		y /= s;
		return *this;
	}

	vec2& rotate(double theta) {
		double c = std::cos(theta);
		double s = std::sin(theta);
		double tx = x * c - y * s;
		double ty = x * s + y * c;
		x = tx;
		y = ty;
		return *this;
	}

	vec2& normalize() {
		if (length() == 0) return *this;
		*this *= (1.0 / length());
		return *this;
	}

	double length() const {
		return std::sqrt(x*x + y*y);
	}
	void truncate(double length) {
		double angle = std::atan2(y, x);
		x = length * std::cos(angle);
		y = length * std::sin(angle);
	}

	vec2 ortho() const {
		return vec2(y, -x);
	}

	static double dist(vec2 const& v1, vec2 const& v2) {
		vec2 d(v1.x - v2.x, v1.y - v2.y);
		return d.length();
	}
	static double dot(vec2 const& v1, vec2 const& v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}
	static double cross(vec2 const& v1, vec2 const& v2) {
		return (v1.x * v2.y) - (v1.y * v2.x);
	}

	std::string str() const {
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}

	std::string json() const {
		std::stringstream ss;
		ss.precision(17);
		ss
			<< "{"
				<< "\"class\":" << "\"vec2\"" << ","
				<< "\"parameters\":"
				<< "{"
					<< "\"x\":" << x << ","
					<< "\"y\":" << y
				<< "}"
			<< "}";
		return ss.str();
	}
};

static std::ostream& operator<<(std::ostream& stream, vec2 const& v) {
	return stream << v.str() << std::endl;
}

#endif
