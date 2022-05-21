from physics import World, Segment, Arc, BezierCubic, Ball, vec2
import numpy

def get_runtime_demo_world() -> World:
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

	world.add_curve(Arc(vec2(1000, 700), 150, 0, numpy.pi))
	world.add_curve(Segment(vec2(850, 700), vec2(850, 500)))
	world.add_curve(Segment(vec2(1150, 700), vec2(1150, 500)))
	world.add_curve(Arc(vec2(1000, 500), 150, numpy.pi, 2*numpy.pi))

	world.add_curve(Arc(vec2(1000, 200), 100, 0, 2*numpy.pi))

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
	for angle in numpy.linspace(0, 2*numpy.pi, 200):
		world.add_ball(Ball(vec2(1000, 600), vec2(numpy.cos(angle), numpy.sin(angle))))
	for angle in numpy.linspace(0, 2*numpy.pi, 200):
		world.add_ball(Ball(vec2(1050, 200), vec2(numpy.cos(angle), numpy.sin(angle))))

	return world


if __name__ == '__main__':
	world = get_runtime_demo_world()
	print(world.json())