#ifndef __WORLD_HPP__
#define __WORLD_HPP__

#include "globals.h" // EPS
#include "ball.hpp"
#include "curve.hpp"
#include "collider.hpp"
#include "logger.hpp"
#include <vector>
#include <unordered_set>
#include <utility>  // std::pair
#include <algorithm> // std::find, std::min_element
#include <memory>
#include <string>

class World {
	// TODO : use unordered_sets instead
	typedef std::shared_ptr<Ball> BallPtr;
	typedef std::shared_ptr<Segment> SegmentPtr;
	typedef std::vector<BallPtr> BallPtrs;
	typedef std::vector<SegmentPtr> SegmentPtrs;

	void integrate(double dt) {
		for (BallPtr const& ball_ptr : ball_ptrs) {
			ball_ptr->pos_prev = ball_ptr->pos;
			ball_ptr->pos += ball_ptr->vel * dt;
		}
	}

	void resolve_collision(Ball& ball) {
		std::vector<std::pair<vec2, SegmentPtr>> inters;
		unsigned int iter_num = 0;

		while (iter_num++ < MAX_COLL_ITERS) {
			inters.clear();
			Segment traj(ball.pos_prev, ball.pos);
			Logger::debug("=== iteration " + std::to_string(iter_num) + " ===");
			Logger::debug(traj.str());

			for (SegmentPtr const& segment_ptr : segment_ptrs) {
				Segment const& seg = *segment_ptr;
				vec2 interpt = Collider::line_line(Segment(ball.pos, ball.pos + ball.vel), seg);  // implicit cast to Line

				// Test if the intersection point lies on Segment(ball.pos_prev, ball.pos)
				if (!(traj.in_bounds(interpt) && seg.in_bounds(interpt)))
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

				Logger::debug(interpt.str());
				inters.push_back(std::make_pair(interpt, segment_ptr));
			}

			if (inters.size() == 0)
				// No collision to be resolved
				break;

			// Compute distance of pos_prev to all intersection points
			std::vector<double> dists(inters.size());
			for (unsigned int i(0); i < inters.size(); ++i) {
				dists[i] = (inters[i].first - ball.pos_prev).length();
			}
			double mindist = *std::min_element(dists.begin(), dists.end());

			// Resolve the collisions with the segments
			for (unsigned int i(0); i < inters.size(); ++i) {
				vec2& interpt = inters[i].first;
				Segment const& seg = *inters[i].second;

				// Find intersection closest to the old ball position within a tolerance
				// This is done to deal with "perfect corner" situations
				if (std::abs(dists[i] - mindist) > EPS)
					continue;

				// Compute the correction
				vec2 diff = ball.pos - interpt;
				vec2 n = seg.ortho().normalize();
				vec2 m = seg.tangent().normalize();
				vec2 newpos = interpt - vec2::dot(n, diff)*n + vec2::dot(m, diff)*m;
				vec2 newvel = -vec2::dot(n, ball.vel)*n + vec2::dot(m, ball.vel)*m;

				// Resolve collision (with the closest line)
				// Snap ball to intersection point. At this point the ball is on the same side as previously
				ball.pos_prev = interpt;
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
	SegmentPtrs segment_ptrs;

	World() = default;

	void step(double dt) {
		integrate(dt);
		resolve_collisions();
	}

	void add_ball(BallPtr ball_ptr) { ball_ptrs.push_back(ball_ptr); }
	void add_segment(SegmentPtr segment_ptr) { segment_ptrs.push_back(segment_ptr); }

	virtual std::string str() const {
		std::string ret;
		ret += "Balls:";
		for (BallPtr const& ball_ptr : ball_ptrs) {
			ret += "\n\t" + ball_ptr->str();
		}
		ret += "\nSegments:";
		for (SegmentPtr const& segment_ptr : segment_ptrs) {
			ret += "\n\t" + segment_ptr->str();
		}
		return ret;
	}
};

#endif
