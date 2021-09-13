# TODO : datafile for the segments and initial configuration
# TODO : ball and line color/thickness etc.
# TODO : temp directory

from chaotic_pool.physics import World, Segment, Ball, vec2
import os
import numpy
from PIL import Image, ImageDraw

def gen_frame(world: World):
	im = Image.new('RGB', (500, 500), (0, 0, 0))
	draw = ImageDraw.Draw(im)

	for segment in world.segments:
		draw.line(
			(segment.p1.x, segment.p1.y, segment.p2.x, segment.p2.y),
			fill=(128, 128, 128),
			width=2
		)

	for ball in world.balls:
		draw.ellipse(
			(ball.pos.x-1, ball.pos.y-1, ball.pos.x+1, ball.pos.y+1),
			fill=(255, 0, 0)
		)

	return im


world = World()
# world.add_segment(Segment(vec2(100, 100), vec2(400, 200)))
# world.add_segment(Segment(vec2(400, 200), vec2(400, 400)))
# world.add_segment(Segment(vec2(400, 400), vec2(50, 350)))
# world.add_segment(Segment(vec2(50, 350), vec2(100, 100)))
# world.add_segment(Segment(vec2(100, 100), vec2(400, 100)))
# world.add_segment(Segment(vec2(400, 100), vec2(400, 400)))
# world.add_segment(Segment(vec2(400, 400), vec2(100, 400)))
# world.add_segment(Segment(vec2(100, 400), vec2(100, 100)))
for angle in numpy.linspace(0, 2*numpy.pi, 100):
	world.add_segment(Segment(
		200*vec2(numpy.cos(angle), numpy.sin(angle)) + vec2(300, 300),
		200*vec2(numpy.cos(angle+2*numpy.pi/99), numpy.sin(angle+2*numpy.pi/99)) + vec2(300, 300)
	))
for angle in numpy.linspace(0, numpy.pi, 10000):
# for angle in [0]:
	world.add_ball(Ball(vec2(200, 300), vec2(numpy.cos(angle), numpy.sin(angle))))

os.system('mkdir -p frames')
os.system('rm frames/frame*.png')

import time

for n in range(N := 1000):
	print(f'>>> ITERATION {n}/{N}')
	tstart = time.perf_counter()
	world.step(5)
	tend = time.perf_counter()
	print(f'simulation took {(tend-tstart)*1e3:.5f}ms')

	tstart = time.perf_counter()
	im = gen_frame(world)
	tend = time.perf_counter()
	im.save(f'frames/frame{n}.png')
	print(f'rendering took {(tend-tstart)*1e3:.5f}ms')

os.system('ffmpeg -framerate 30 -i frames/frame%d.png -c:v libx264 -r 30 -pix_fmt yuv420p out.mp4')
