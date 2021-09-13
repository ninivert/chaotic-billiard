from chaotic_pool.physics import Segment, vec2

print('>>> initializing segments')
s1 = Segment(vec2(-1, 0), vec2(1, 0))
s2 = Segment(vec2(0, -1), vec2(0, 1))
print(s1, s2, sep='\n')

print('>>> testing coefs')
assert(s1.coefs().p == 0.0)
assert(s1.coefs().q == 2.0)
assert(s1.coefs().r == 0.0)
assert(s2.coefs().p == -2.0)
assert(s2.coefs().q == 0.0)
assert(s2.coefs().r == 0.0)
print(s1.coefs(), s2.coefs(), sep='\n')
print('OK')

print('>>> intersecting')
assert(Segment.intersect(s1, s2).point.x == vec2(0.0, 0.0).x)  # TODO : __eq__ operator
assert(Segment.intersect(s1, s2).point.y == vec2(0.0, 0.0).y)
assert(Segment.intersect(s1, s2).on_segment == 1)
print(Segment.intersect(s1, s2))
print('OK')
