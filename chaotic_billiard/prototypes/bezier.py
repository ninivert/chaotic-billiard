import pyglet
from pyglet.gl import *
from chaotic_billiard.physics import vec2, Segment, Ball, constants
from typing import List
import numpy

# TODO : implement in c++
class BezierCubic:
	def __init__(self, p0: vec2, p1: vec2, p2: vec2, p3: vec2):
		self.p0 = p0
		self.p1 = p1
		self.p2 = p2
		self.p3 = p3

	def __call__(self, t, order=0):
		# https://en.wikipedia.org/wiki/B%C3%A9zier_curve#Cubic_B%C3%A9zier_curves
		assert order >= 0, 'order must be positive or null'
		assert 0 <= t <= 1, 't must lie between 0 and 1'
		if order == 0:
			return \
				(1-t)**3 * self.p0 + \
				3*(1-t)**2 * t * self.p1 + \
				3*(1-t) * t**2 * self.p2 + \
				t**3 * self.p3
		if order == 1:
			return \
				3*(1-t)**2 * (self.p1 - self.p0) + \
				6*(1-t) * t * (self.p2 - self.p1) + \
				3*t**2 * (self.p3 - self.p2)
		if order == 2:
			return \
				6*(1-t)*(self.p2 - 2*self.p1 + self.p0) + \
				6*t * (self.p3 - 2*self.p2 + self.p1)

	def tangent(self, t):
		assert 0 <= t <= 1, 't must lie between 0 and 1'
		return self(t, order=1).normalize()

	def ortho(self, t):
		assert 0 <= t <= 1, 't must lie between 0 and 1'
		return self.tangent(t).ortho()


# TODO : parametrize segments and other collidable shapes
# TODO : segment check

def inter_seg_bezier(seg: Segment, bezier: BezierCubic) -> List[vec2]:
	# https://stackoverflow.com/questions/14005096/mathematical-solution-for-bezier-curve-and-line-intersection-in-coffeescript-or
	bezier_polycoefs = lambda a, b, c, d: [-a + 3*b - 3*c + d, 3*a - 6*b + 3*c, -3*a + 3*b, a]
	k = seg.coefs()
	bx = bezier_polycoefs(bezier.p0.x, bezier.p1.x, bezier.p2.x, bezier.p3.x)
	by = bezier_polycoefs(bezier.p0.y, bezier.p1.y, bezier.p2.y, bezier.p3.y)

	poly = numpy.polynomial.Polynomial(
		[
			k.p*bx[3] + k.q*by[3] - k.r,
			k.p*bx[2] + k.q*by[2],
			k.p*bx[1] + k.q*by[1],
			k.p*bx[0] + k.q*by[0],
		],
		(0, 1),
		(0, 1)
	)

	t_roots = poly.roots()

	return [t.real for t in t_roots if 0 <= t.real <= 1 and abs(t.imag) < 1e-12]


BEZIER_SAMPLE_SIZE = 100
PHYSICS_SPEED = 1000
DRAW_DEBUG_SEG = False
DRAW_DEBUG_COLL = False
DRAW_DEBUG_COLL_BASIS = False
DRAW_DEBUG_COLL_POINT = False
DRAW_DEBUG_TRAJ = True
DRAW_DEBUG_QUEUED_COLL_POINT = True
MANUAL_FRAME = False  # press space to advance
MAX_COLL_ITERS = 1000

beziers = set()
beziers.add(BezierCubic(vec2(100, 100), vec2(200, 500), vec2(400, 200), vec2(500, 100)))
beziers.add(BezierCubic(vec2(700, 50), vec2(-100, 600), vec2(1000, 600), vec2(50, 100)))
beziers.add(BezierCubic(vec2(200, 50), vec2(200, 50), vec2(800, 200), vec2(800, 200)))
seg = Segment(vec2(50, 200), vec2(500, 300))
balls = set()
balls.add(Ball(vec2(200, 200), vec2(1, 1).normalize()))
balls.add(Ball(vec2(390, 300), vec2(2, 0).normalize()))
balls.add(Ball(vec2(410, 380), vec2(2, 0).normalize()))
window = pyglet.window.Window(width=900, height=600)

def step(dt=0.1):
	global seg, beziers, balls

	# Physics

	for ball in balls:
		ball.pos_prev = vec2(ball.pos)
		ball.pos += ball.vel*dt*PHYSICS_SPEED

	print('========= NEW STEP =========')
	for ball in balls:
		iter_num = 0

		if DRAW_DEBUG_TRAJ:
			glLineWidth(1)
			pyglet.graphics.draw(2, GL_LINES, ('v2f', [*ball.pos_prev, *ball.pos]), ('c3B', (255, 128, 128)*2))
			glPointSize(3)
			pyglet.graphics.draw(1, GL_POINTS, ('v2f', [*ball.pos_prev]), ('c3B', (255, 128, 128)))

		while iter_num < MAX_COLL_ITERS:
			print(f'=== iteration {iter_num} ===')
			print(ball, f'pos_prev={ball.pos_prev}')
			inters = []

			for bezier in beziers:
				# velocity is by definition the tangential direction of movement
				ts = inter_seg_bezier(Segment(ball.pos, ball.pos+ball.vel), bezier)
				for t in ts:
					interpt = bezier(t)

					if not Segment(ball.pos_prev, ball.pos).in_bounds(interpt):
						continue
					print('interpt dist', (interpt - ball.pos_prev).length())
					if (interpt - ball.pos_prev).length() < constants.eps:
						continue

					if DRAW_DEBUG_QUEUED_COLL_POINT:
						glPointSize(10)
						pyglet.graphics.draw(1, GL_POINTS, ('v2f', [*interpt]), ('c3B', (255, 0, 0)))

					print('interpt', interpt)
					inters.append((t, bezier))

			if len(inters) == 0:
				break

			dists = [(bezier(t) - ball.pos_prev).length() for (t, bezier) in inters]
			print('dists', dists)

			for i, (t, bezier) in enumerate(inters):
				if abs(dists[i] - min(dists)) > constants.eps:
					continue

				n = bezier.ortho(t)
				m = bezier.tangent(t)
				interpt = bezier(t)

				diff = ball.pos - interpt
				newpos = interpt - vec2.dot(n, diff)*n + vec2.dot(m, diff)*m
				newvel = -vec2.dot(n, ball.vel)*n + vec2.dot(m, ball.vel)*m

				ball.pos_prev = interpt
				ball.pos = newpos
				ball.vel = newvel

				if DRAW_DEBUG_TRAJ:
					glLineWidth(1)
					pyglet.graphics.draw(2, GL_LINES, ('v2f', [*ball.pos_prev, *ball.pos]), ('c3B', (128, 255, 128)*2))
					glPointSize(5)
					pyglet.graphics.draw(1, GL_POINTS, ('v2f', [*ball.pos_prev]), ('c3B', (128, 128, 255)))

			iter_num += 1
			if iter_num == MAX_COLL_ITERS-1:
				print(f'max iterations on collision resolution for {ball}')

def draw(dt=0.1):
	global window, seg, beziers, balls

	for bezier in beziers:
		# Draw Bezier curve
		vertices = []
		for i in range(BEZIER_SAMPLE_SIZE):
			vertices += bezier(i/(BEZIER_SAMPLE_SIZE-1))

		# opengl weirdness
		vertices.insert(0, vertices[0])
		vertices.insert(1, vertices[2])
		vertices += vertices[-2:]

		glLineWidth(1)
		pyglet.graphics.draw(BEZIER_SAMPLE_SIZE+2, GL_LINE_STRIP, ('v2f', vertices))

		# Draw control points
		# glPointSize(5)
		# pyglet.graphics.draw(4, GL_POINTS, ('v2f', [*bezier.p0, *bezier.p1, *bezier.p2, *bezier.p3]), ('c3B', (255, 0, 0)*4))

		t_inters = []
		if DRAW_DEBUG_SEG:
			# Draw the intersection points, tangent and ortho
			t_inters += inter_seg_bezier(seg, bezier)

		if DRAW_DEBUG_COLL:
			for ball in balls:
				t_inters += inter_seg_bezier(Segment(ball.pos, ball.pos+ball.vel), bezier)

		if DRAW_DEBUG_COLL_POINT:
			vertices = []
			for t in t_inters:
				vertices += bezier(t)
			glPointSize(5)
			pyglet.graphics.draw(len(t_inters), GL_POINTS, ('v2f', vertices), ('c3B', (0, 128, 255)*len(t_inters)))

		# Tangent and normal vectors
		if DRAW_DEBUG_COLL_BASIS:
			for t in t_inters:
				m = bezier.tangent(t)*40
				n = bezier.ortho(t)*40
				pt = bezier(t)
				glLineWidth(1)
				pyglet.graphics.draw(2, GL_LINES, ('v2f', [*pt, *(pt+m)]), ('c3B', (255, 255, 0)*2))
				pyglet.graphics.draw(2, GL_LINES, ('v2f', [*pt, *(pt+n)]), ('c3B', (255, 128, 0)*2))

	if DRAW_DEBUG_SEG:
		# Draw the seg
		glPointSize(5)
		glLineWidth(1)
		pyglet.graphics.draw(2, GL_POINTS, ('v2f', [*seg.p1, *seg.p2]), ('c3B', (128, 0, 128)*2))
		pyglet.graphics.draw(2, GL_LINES, ('v2f', [*seg.p1, *seg.p2]), ('c3B', (128, 128, 128)*2))

	for ball in balls:
		# Draw the balls
		glPointSize(5)
		pyglet.graphics.draw(1, GL_POINTS, ('v2f', [*ball.pos]), ('c3B', (0, 255, 128)))


@window.event
def on_mouse_press(x, y, button, modifiers):
	global seg
	print(x, y)
	if button == pyglet.window.mouse.LEFT:
		seg.p1 = vec2(x, y)
		draw()
	if button == pyglet.window.mouse.RIGHT:
		seg.p2 = vec2(x, y)
		draw()

@window.event
def on_key_press(symbol, modifiers):
	if MANUAL_FRAME:
		if symbol == pyglet.window.key.SPACE:
			window.clear()
			step(0.1)
			draw(0.1)
		if symbol == pyglet.window.key.BACKSPACE:
			window.clear()
			step(-0.1)
			draw(-0.1)

def step_and_draw(dt=0.1):
	window.clear()
	step(dt)
	draw(dt)

if not MANUAL_FRAME:
	pyglet.clock.schedule_interval(step_and_draw, 1/60.0)
pyglet.app.run()
