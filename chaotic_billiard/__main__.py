# TODO : datafile for the segments and initial configuration
# TODO : ball and line color/thickness etc.
# TODO : temp directory

from chaotic_billiard.physics import World, Segment, BezierCubic, Ball, vec2
import numpy
import pyglet
from pyglet.gl import *

world = World()

world.add_curve(Segment(vec2(500, 100), vec2(800, 200)))
world.add_curve(Segment(vec2(800, 200), vec2(800, 400)))
world.add_curve(Segment(vec2(800, 400), vec2(450, 350)))
world.add_curve(Segment(vec2(450, 350), vec2(500, 100)))

world.add_curve(Segment(vec2(100, 100), vec2(400, 100)))
world.add_curve(Segment(vec2(400, 100), vec2(400, 400)))
world.add_curve(Segment(vec2(400, 400), vec2(100, 400)))
world.add_curve(Segment(vec2(100, 400), vec2(100, 100)))
# world.add_curve(Segment(vec2(300, 300), vec2(350, 350)))

world.add_curve(BezierCubic(vec2(700, 450), vec2(-100, 1000), vec2(1000, 1000), vec2(50, 500)))
world.add_curve(Segment(vec2(700, 450), vec2(50, 500)))

# for angle in numpy.linspace(0, 2*numpy.pi, 100):
# 	world.add_segment(Segment(
# 		200*vec2(numpy.cos(angle), numpy.sin(angle)) + vec2(300, 300),
# 		200*vec2(numpy.cos(angle+2*numpy.pi/99), numpy.sin(angle+2*numpy.pi/99)) + vec2(300, 300)
# 	))

for angle in numpy.linspace(0, 2*numpy.pi, 200):
	world.add_ball(Ball(vec2(250, 250), vec2(numpy.cos(angle), numpy.sin(angle))))
for angle in numpy.linspace(0, 2*numpy.pi, 200):
	world.add_ball(Ball(vec2(600, 250), vec2(numpy.cos(angle), numpy.sin(angle))))
for angle in numpy.linspace(0, 2*numpy.pi, 200):
	world.add_ball(Ball(vec2(420, 650), vec2(numpy.cos(angle), numpy.sin(angle))))
for angle in numpy.linspace(0, 2*numpy.pi, 200):
	world.add_ball(Ball(vec2(420, 820), vec2(numpy.cos(angle), numpy.sin(angle))))

window = pyglet.window.Window(width=900, height=900)

BEZIER_SAMPLE_SIZE = 100

def step(dt):
	world.step(dt)

def draw():
	glClearColor(0.0, 0.0, 0.0, 0.0)
	glClear(GL_COLOR_BUFFER_BIT)

	glPointSize(1)
	glColor3f(0.0, 1.0, 0.5)
	glBegin(GL_POINTS)
	for ball in world.balls:
		glVertex2f(*ball.pos)
	glEnd()

	segs = [curve for curve in world.curves if isinstance(curve, Segment)]
	glLineWidth(1)
	glColor3f(0.5, 0.5, 0.5)
	glBegin(GL_LINES)
	for seg in segs:
		glVertex2f(*seg.p1)
		glVertex2f(*seg.p2)
	glEnd()

	beziers = [curve for curve in world.curves if isinstance(curve, BezierCubic)]
	glLineWidth(1)
	glColor3f(0.5, 0.5, 0.5)
	for bezier in beziers:
		glBegin(GL_LINE_STRIP)
		glVertex2f(*bezier(0))
		for i in range(BEZIER_SAMPLE_SIZE):
			glVertex2f(*bezier(i/(BEZIER_SAMPLE_SIZE-1)))
		glVertex2f(*bezier(1))
		glEnd()

def step_and_draw(dt):
	import time
	tstart1 = time.perf_counter()
	step(dt*100)
	tend1 = time.perf_counter()

	tstart2 = time.perf_counter()
	draw()
	tend2 = time.perf_counter()

	print(f'stepped dt={int(dt*1e3):2d} in {(tend1-tstart1)*1e3:.2f}ms, rendered in {int((tend2-tstart2)*1e3):2d}ms')

pyglet.clock.schedule_interval(step_and_draw, 1/60.0)
pyglet.app.run()


# os.system('mkdir -p frames')
# os.system('rm frames/frame*.png')

# import time

# for n in range(N := 1000):
# 	print(f'>>> ITERATION {n}/{N}')
# 	tstart = time.perf_counter()
# 	world.step(5)
# 	tend = time.perf_counter()
# 	print(f'simulation took {(tend-tstart)*1e3:.5f}ms')

# 	tstart = time.perf_counter()
# 	render(world)
# 	tend = time.perf_counter()
# 	print(f'rendering took {(tend-tstart)*1e3:.5f}ms')

# os.system('ffmpeg -framerate 30 -i frames/frame%d.png -c:v libx264 -r 30 -pix_fmt yuv420p out.mp4')
