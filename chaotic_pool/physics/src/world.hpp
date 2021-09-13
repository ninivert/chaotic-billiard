#ifndef __WORLD_HPP__
#define __WORLD_HPP__

#include "globals.h" // EPS
#include "ball.hpp"
#include "segment.hpp"
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

	void resolve_collisions() {
		for (BallPtr& ball_ptr : ball_ptrs) {
			std::vector<SegmentPtr> ignored_segments;
			std::vector<std::pair<vec2, SegmentPtr>> inters;

			while (true) {  // TODO : this is dirty, change it
				// Compute all intersection points with the lines in the trajectory and test if it lies on both segments
				inters.clear();
				for (SegmentPtr const& segment_ptr : segment_ptrs) {
					// Skip ignored segments
					if (std::find(ignored_segments.begin(), ignored_segments.end(), segment_ptr) != ignored_segments.end())
						continue;

					Segment balltraj(ball_ptr->pos_prev, ball_ptr->pos);
					SegmentIntersection inter = Segment::intersect(balltraj, *segment_ptr);

					if (inter.on_segment) {
						// Collision excludes prev_pos, otherwise it can happen that collision is triggered
						// by pos, then in the next physics step by pos_prev
						if ((inter.point - ball_ptr->pos_prev).length() > EPS) {
							inters.push_back(std::make_pair(inter.point, segment_ptr));
						}
					}
				}

				if (inters.size() == 0)
					// No collision to be resolved
					break;

				// Compute distance of pos_prev to all intersection points
				std::vector<double> dists(inters.size());
				for (unsigned int i(0); i < inters.size(); ++i) {
					dists[i] = (inters[i].first - ball_ptr->pos_prev).length();
				}
				double mindist = *std::min_element(dists.begin(), dists.end());

				// Resolve the collisions with the segments
				ignored_segments.clear();
				for (unsigned int i(0); i < inters.size(); ++i) {
					vec2& interpt = inters[i].first;
					SegmentPtr& segment_ptr = inters[i].second;

					// Find intersection closest to the old ball position within a tolerance
					// This is done to deal with "perfect corner" situations
					if (std::abs(dists[i] - mindist) > EPS)
						continue;

					// Compute the correction
					vec2 diff = ball_ptr->pos - interpt;
					vec2 n = segment_ptr->ortho().normalize();
					vec2 m = segment_ptr->tangent().normalize();
					vec2 newpos = interpt - vec2::dot(n, diff)*n + vec2::dot(m, diff)*m;
					vec2 newvel = -vec2::dot(n, ball_ptr->vel)*n + vec2::dot(m, ball_ptr->vel)*m;

					// WARNING : diff can be zero !!
					// This happens when a ball lands perfectly on the line
					// In that case, the pos_prev and pos are the same,
					// making the line coefficients (and the determinant) zero of the intersection check in the next iteration
					// HACK : add 1e-3*newvel to oldpos to keep the direction (or add a vec2 direction to the ball)
					// The hack doesn't work because on the next physics iteration, pos_prev will be the colliding pos
					// triggering collision handling again, and moving the point to the other side
					// To fix this, say the ball trajectory is a segment that excludes pos_prev

					// Resolve collision (with the closest line)
					// Snap ball to intersection point. At this point the ball is on the same side as previously
					ball_ptr->pos_prev = interpt
						+ 1e-3*newvel;  // HACK : fix me with direction unit vector
					ball_ptr->pos = newpos;
					ball_ptr->vel = newvel;

					// Since pos_prev is on the line, the next iteration will always consider that collision point and the program hangs,
					// so add the segment to the list of ignored segments
					ignored_segments.push_back(segment_ptr);

					// Next iteration resolves the rest of the collisions (ball may have crossed multiple lines in one step)
				}
			}
		}
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
