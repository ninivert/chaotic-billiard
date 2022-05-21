# TODO : datafile for the segments and initial configuration
# TODO : ball and line color/thickness etc.
# TODO : temp directory

from physics import World, Segment, Arc, BezierCubic, Ball, vec2

if __name__ == '__main__':
	import pyglet
	from pyglet.gl import *
	import sys

	if len(sys.argv) > 1:
		from demo_from_worldfile import world_from_dict
		import json

		with open(sys.argv[1]) as file:
			world_dict = json.load(file)
		
		world = world_from_dict(world_dict)
	else:
		from demo_to_worldfile import get_runtime_demo_world
		world = get_runtime_demo_world()

	window = pyglet.window.Window(width=1200, height=900)

	CURVE_SAMPLE_SIZE = 100

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

		explcurves = [curve for curve in world.curves if isinstance(curve, BezierCubic) or isinstance(curve, Arc)]
		glLineWidth(1)
		glColor3f(0.5, 0.5, 0.5)
		for explcurve in explcurves:
			glBegin(GL_LINE_STRIP)
			glVertex2f(*explcurve(0))
			for i in range(CURVE_SAMPLE_SIZE):
				glVertex2f(*explcurve(i/(CURVE_SAMPLE_SIZE-1)))
			glVertex2f(*explcurve(1))
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
