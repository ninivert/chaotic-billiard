#ifndef __WORLD_HPP__
#define __WORLD_HPP__

#include "globals.h" // EPS
#include "ball.hpp"
#include "curve.hpp"
#include "collider.hpp"
#include "logger.hpp"
#include <vector>
#include <unordered_set>
#include <algorithm>  // std::min_element
#include <sstream>  // std::stringstream
#include <memory>
#include <string>

class World {
	typedef std::shared_ptr<Ball> BallPtr;
	typedef std::shared_ptr<Curve> CurvePtr;
	typedef std::vector<BallPtr> BallPtrs;
	typedef std::vector<CurvePtr> CurvePtrs;

	struct Inter {
		double t;
		vec2 interpt;
		CurvePtr curve_ptr;
	};

	void integrate(double dt) {
		for (BallPtr const& ball_ptr : ball_ptrs) {
			ball_ptr->pos_prev = ball_ptr->pos;
			ball_ptr->pos += ball_ptr->vel * dt;
		}
	}

	void resolve_collision(Ball& ball) {
		std::vector<Inter> inters;
		unsigned int iter_num = 0;

		while (iter_num++ < MAX_COLL_ITERS) {
			inters.clear();
			Segment traj(ball.pos_prev, ball.pos);
			Segment dir(ball.pos, ball.pos + ball.vel);
			// Logger::debug("=== iteration " + std::to_string(iter_num) + " ===");
			// Logger::debug(traj.str());

			for (CurvePtr const& curve_ptr : curve_ptrs) {
				Collider::ParamPairs tpairs(dir.collide(*curve_ptr));
				for (Collider::ParamPair& tpair : tpairs) {
					vec2 interpt((*curve_ptr)(tpair.t2));
					tpair.t1 = traj.inverse(interpt);  // substitute with t on the trajectory
					// Logger::debug("candidate " + tpair.str() + " collision at " + interpt.str() + " with " + curve_ptr->str());

					// Test if the intersection point lies on Segment(ball.pos_prev, ball.pos)
					if (!tpair.on_both())
						continue;

					// WARNING : diff can be zero !!
					// This happens when a ball lands perfectly on the line
					// In that case, the pos_prev and pos are the same,
					// making the line coefficients (and the determinant) zero of the intersection check in the next iteration
					// FIX : using ball.vel to give the orientation of the trajectory, instead of the Segment(ball.pos_prev, ball.pos)
					// But the fix doesn't work because on the next physics iteration, pos_prev will be the colliding pos
					// triggering collision handling again, and moving the point to the other side
					// To fix this, say the ball trajectory is a segment that excludes pos_prev
					if ((interpt - ball.pos_prev).length() < EPS)
						continue;

					// Logger::debug("selected " + tpair.str() + " collision at " + interpt.str() + " with " + curve_ptr->str());
					inters.push_back(Inter{tpair.t2, interpt, curve_ptr});
				}
			}

			if (inters.size() == 0)
				// No collision to be resolved
				break;

			// Compute distance of pos_prev to all intersection points
			std::vector<double> dists(inters.size());
			for (unsigned int i(0); i < inters.size(); ++i) {
				dists[i] = (inters[i].interpt - ball.pos_prev).length();
			}
			double mindist = *std::min_element(dists.begin(), dists.end());

			// Resolve the collisions with the curves
			for (unsigned int i(0); i < inters.size(); ++i) {
				// Find intersection closest to the old ball position within a tolerance
				// This is done to deal with "perfect corner" situations
				// PITFALL : However, this comes with the cost that (for example)
				// two vertical Segments in the same position will let everything through (double collision in one frame)
				if (std::abs(dists[i] - mindist) > EPS)
					continue;

				// Compute the correction
				vec2 diff = ball.pos - inters[i].interpt;
				vec2 n = inters[i].curve_ptr->ortho(inters[i].t).normalize();
				vec2 m = inters[i].curve_ptr->tangent(inters[i].t).normalize();
				vec2 newpos = inters[i].interpt - vec2::dot(n, diff)*n + vec2::dot(m, diff)*m;
				vec2 newvel = -vec2::dot(n, ball.vel)*n + vec2::dot(m, ball.vel)*m;

				// Resolve collision (with the closest line)
				// Snap ball to intersection point. At this point the ball is on the same side as previously
				ball.pos_prev = inters[i].interpt;
				ball.pos = newpos;
				ball.vel = newvel;

				// Next iteration resolves the rest of the collisions (ball may have crossed multiple lines in one step)
			}
		}
	}

	void resolve_collisions() {
		for (BallPtr& ball_ptr : ball_ptrs)
			resolve_collision(*ball_ptr);
	}

public:
	// pybind11 needs to read these, so making public
	BallPtrs ball_ptrs;
	CurvePtrs curve_ptrs;

	World() = default;

	void step(double dt) {
		integrate(dt);
		resolve_collisions();
	}

	void add_ball(BallPtr ball_ptr) { ball_ptrs.push_back(ball_ptr); }
	void add_curve(CurvePtr curve_ptr) { curve_ptrs.push_back(curve_ptr); }

	virtual std::string str() const {
		std::string ret;
		ret += "Balls:";
		for (BallPtr const& ball_ptr : ball_ptrs) {
			ret += "\n\t" + ball_ptr->str();
		}
		ret += "\nCurves:";
		for (CurvePtr const& curve_ptr : curve_ptrs) {
			ret += "\n\t" + curve_ptr->str();
		}
		return ret;
	}

	std::string json() const {
		std::stringstream ss;
		ss
			<< "{"
				<< "\"balls\":[";
					for (BallPtr const& ball_ptr : ball_ptrs)
						ss << ball_ptr->json() << ",";
					ss.seekp(-1, ss.cur);  // override the last comma
				ss << "]" << ","
				<< "\"curves\":[";
					for (CurvePtr const& curve_ptr : curve_ptrs)
						ss << curve_ptr->json() << ",";
					ss.seekp(-1, ss.cur);  // override the last comma
				ss << "]"
			<< "}";
		return ss.str();
	}

	// TODO
	// static World from_json(std::string const& json);
};

#endif
