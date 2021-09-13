# TODO : datafile for the segments and initial configuration
# TODO : ball and line color/thickness etc.
# TODO : temp directory

from chaotic_pool.physics import World, Segment, Ball, vec2
import numpy
import pyglet
from pyglet.gl import *

world = World()
# world.add_segment(Segment(vec2(100, 100), vec2(400, 200)))
# world.add_segment(Segment(vec2(400, 200), vec2(400, 400)))
# world.add_segment(Segment(vec2(400, 400), vec2(50, 350)))
# world.add_segment(Segment(vec2(50, 350), vec2(100, 100)))
world.add_segment(Segment(vec2(100, 100), vec2(400, 100)))
world.add_segment(Segment(vec2(400, 100), vec2(400, 400)))
world.add_segment(Segment(vec2(400, 400), vec2(100, 400)))
world.add_segment(Segment(vec2(100, 400), vec2(100, 100)))
world.add_segment(Segment(vec2(300, 300), vec2(350, 350)))
# for angle in numpy.linspace(0, 2*numpy.pi, 100):
# 	world.add_segment(Segment(
# 		200*vec2(numpy.cos(angle), numpy.sin(angle)) + vec2(300, 300),
# 		200*vec2(numpy.cos(angle+2*numpy.pi/99), numpy.sin(angle+2*numpy.pi/99)) + vec2(300, 300)
# 	))
for angle in numpy.linspace(0, numpy.pi, 1000):
	world.add_ball(Ball(vec2(200, 325), vec2(numpy.cos(angle), numpy.sin(angle))))


window = pyglet.window.Window(width=900, height=600)

def step(dt):
	world.step(dt)

def draw():
	window.clear()

	vertices = []
	for ball in world.balls:
		vertices += ball.pos
	glPointSize(1)
	pyglet.graphics.draw(len(world.balls), GL_POINTS, ('v2f', vertices), ('c3B', (0, 255, 128)*(len(world.balls))))

	vertices = []
	for seg in world.segments:
		vertices += seg.p1
		vertices += seg.p2
	glLineWidth(1)
	pyglet.graphics.draw(len(world.segments)*2, GL_LINES, ('v2f', vertices), ('c3B', (128, 128, 128)*len(world.segments)*2))

def step_and_draw(dt):
	import time
	tstart1 = time.perf_counter()
	step(dt*100)
	tend1 = time.perf_counter()

	tstart2 = time.perf_counter()
	draw()
	tend2 = time.perf_counter()

	print(f'stepped {dt:3f} in {(tend1-tstart1)*1e3:5f}ms, rendered in {(tend2-tstart2)*1e3:5f}ms')

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
