from physics import Segment, Line, vec2

print('>>> initializing segments')
s1 = Segment(vec2(-1, 0), vec2(1, 0))
s2 = Segment(vec2(0, -1), vec2(0, 1))
print(s1, s2, sep='\n')

print('>>> casting to Line')
l1, l2 = s1.to_line(), s2.to_line()
print(l1, l2, sep='\n')

print('>>> testing coefs')
assert(l1.p == 0.0)
assert(l1.q == 2.0)
assert(l1.r == 0.0)
assert(l2.p == -2.0)
assert(l2.q == 0.0)
assert(l2.r == 0.0)
# print(l1.coefs(), l2.coefs(), sep='\n')
print('OK')

print('>>> intersecting')
col = s1.collide(s2)
print('collision at', col)
p1, p2 = s1(col[0].t1), s2(col[0].t2)
print('to points', p1, p2)
assert p1.x == p2.x
assert p1.y == p2.y
print('OK')
