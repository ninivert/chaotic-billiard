from chaotic_pool.physics import World, Segment, Ball, vec2

w = World()
w.add_segment(Segment(vec2(2, 1), vec2(2, 4)))
w.add_segment(Segment(vec2(1, 2), vec2(4, 2)))
w.add_ball(Ball(vec2(-1, 1), vec2(2, 1)))  # normal collision
w.add_ball(Ball(vec2(0, 1), vec2(2, 1)))  # "perfect corner"

print('>>> initial world')
print(w)

print('>>> stepping')
w.step(10)
print(w)
assert(w.balls[0].pos.x == -15.0)
assert(w.balls[0].pos.y == -7.0)
assert(w.balls[0].vel.x == -2.0)
assert(w.balls[0].vel.y == -1.0)
assert(w.balls[1].pos.x == -16.0)
assert(w.balls[1].pos.y == -7.0)
assert(w.balls[1].vel.x == -2.0)
assert(w.balls[1].vel.y == -1.0)
print('OK')
