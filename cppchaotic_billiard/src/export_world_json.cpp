#include <fstream>
#include <string>
#include <memory>
#include <random>

#include "world.hpp"
#include "ball.hpp"
#include "curve.hpp"
#include "vec2.hpp"
#include "globals.h"

void save(std::string const& filename, World const& world) {
	std::ofstream file;
	file.open(filename);
	file << world.json() << std::endl;
	file.close();
}

int main() {
	World world;

	world = World();
	world.add_curve(std::make_shared<Arc>(vec2(250, 250), 200, 0, 2*M_PI));
	for (double y : Globals::linspace(51, 449, 100000))
		world.add_ball(std::make_shared<Ball>(vec2(250, y), vec2(1, 0)));
	save("world_circle.json", world);

	world = World();
	world.add_curve(std::make_shared<Arc>(vec2(250, 250), 200, 0, 2*M_PI));
	for (double angle : Globals::linspace(0, 2*M_PI, 50000))
		world.add_ball(std::make_shared<Ball>(vec2(300, 250), vec2(std::cos(angle), std::sin(angle))));
	save("world_circle2.json", world);

	world = World();
	world.add_curve(std::make_shared<Arc>(vec2(250, 250), 200, 0, 2*M_PI));
	for (double x : Globals::linspace(250-std::sin(M_PI/4)*200+1e-10, 250+std::sin(M_PI/4)*200-1e-10, 1000)) {
		world.add_ball(std::make_shared<Ball>(vec2(x, 250-std::sin(M_PI/4)*200), vec2(1, 0)));
		world.add_ball(std::make_shared<Ball>(vec2(250-std::sin(M_PI/4)*200, x), vec2(1, 0)));
	}
	save("world_circle3.json", world);

	world = World();
	world.add_curve(std::make_shared<Arc>(vec2(250, 250), 200, 0, 2*M_PI));
	for (double x : Globals::linspace(250-std::sin(M_PI/4)*200+1e-10, 250+std::sin(M_PI/4)*200-1e-10, 1000)) {
		world.add_ball(std::make_shared<Ball>(vec2(x, 250-std::sin(M_PI/4)*200), vec2(1, 0)));
		world.add_ball(std::make_shared<Ball>(vec2(250-std::sin(M_PI/4)*200, x), vec2(0, -1)));
	}
	for (double y : Globals::linspace(51, 449, 10000))
		world.add_ball(std::make_shared<Ball>(vec2(250, y), vec2(1, 0)));
	save("world_circle4.json", world);

	world = World();
	world.add_curve(std::make_shared<Arc>(vec2(150, 250), 100, M_PI/2, 3*M_PI/2));
	// TODO : collision bugs !
	world.add_curve(std::make_shared<Arc>(vec2(350, 250), 100, 3*M_PI/2, M_PI/2));
	// world.add_curve(std::make_shared<Arc>(vec2(350, 250), 100, 0, M_PI/2));
	// world.add_curve(std::make_shared<Arc>(vec2(350, 250), 100, 3*M_PI/2, 2*M_PI));
	world.add_curve(std::make_shared<Segment>(vec2(150, 150), vec2(350, 150)));
	world.add_curve(std::make_shared<Segment>(vec2(150, 350), vec2(350, 350)));
	for (double angle : Globals::linspace(0, 2*M_PI, 10000))
		world.add_ball(std::make_shared<Ball>(vec2(250, 250), vec2(std::cos(angle), std::sin(angle))));
	save("world_capsule.json", world);

	world = World();
	world.add_curve(std::make_shared<Segment>(vec2(50, 50), vec2(450, 50)));
	world.add_curve(std::make_shared<Segment>(vec2(450, 50), vec2(450, 450)));
	world.add_curve(std::make_shared<Segment>(vec2(450, 450), vec2(50, 450)));
	world.add_curve(std::make_shared<Segment>(vec2(50, 450), vec2(50, 50)));
	for (double angle : Globals::linspace(0, 2*M_PI, 100000))
		world.add_ball(std::make_shared<Ball>(vec2(250, 250), vec2(std::cos(angle), std::sin(angle))));
	save("world_square.json", world);

	world = World();
	world.add_curve(std::make_shared<Segment>(vec2(50, 50), vec2(450, 50)));
	world.add_curve(std::make_shared<Segment>(vec2(450, 50), vec2(450, 450)));
	world.add_curve(std::make_shared<Segment>(vec2(450, 450), vec2(50, 450)));
	world.add_curve(std::make_shared<Segment>(vec2(50, 450), vec2(50, 50)));
	world.add_curve(std::make_shared<Arc>(vec2(250, 250), 50, 0, 2*M_PI));
	for (double angle : Globals::linspace(-0.1, 0.1, 10000))
		world.add_ball(std::make_shared<Ball>(vec2(350, 250), vec2(std::cos(angle), std::sin(angle))));
	save("world_sinai.json", world);

	world = World();
	world.add_curve(std::make_shared<Segment>(vec2(50, 50), vec2(450, 50)));
	world.add_curve(std::make_shared<Segment>(vec2(450, 50), vec2(450, 450)));
	world.add_curve(std::make_shared<Segment>(vec2(450, 450), vec2(50, 450)));
	world.add_curve(std::make_shared<Segment>(vec2(50, 450), vec2(50, 50)));
	std::mt19937 rng;
	rng.seed(0);
	std::uniform_real_distribution<double> rpos(60, 440);
	std::normal_distribution<double> rrad(15, 5);
	for (unsigned int i(0); i < 10; ++i) {
		double X = rpos(rng);
		double Y = rpos(rng);
		double R = std::abs(rrad(rng));
		world.add_curve(std::make_shared<Arc>(vec2(X, Y), R, 0, 2*M_PI));
	}
	for (double angle : Globals::linspace(M_PI/2, M_PI, 20000))
		world.add_ball(std::make_shared<Ball>(vec2(449, 51), vec2(std::cos(angle), std::sin(angle))));
	save("world_pinball.json", world);

	world = World();
	world.add_curve(std::make_shared<Segment>(vec2(500, 100), vec2(800, 200)));
	world.add_curve(std::make_shared<Segment>(vec2(800, 200), vec2(800, 400)));
	world.add_curve(std::make_shared<Segment>(vec2(800, 400), vec2(450, 350)));
	world.add_curve(std::make_shared<Segment>(vec2(450, 350), vec2(500, 100)));
	world.add_curve(std::make_shared<Segment>(vec2(100, 100), vec2(400, 100)));
	world.add_curve(std::make_shared<Segment>(vec2(400, 100), vec2(400, 400)));
	world.add_curve(std::make_shared<Segment>(vec2(400, 400), vec2(100, 400)));
	world.add_curve(std::make_shared<Segment>(vec2(100, 400), vec2(100, 100)));
	world.add_curve(std::make_shared<BezierCubic>(vec2(700, 450), vec2(-100, 1000), vec2(1000, 1000), vec2(50, 500)));
	world.add_curve(std::make_shared<Segment>(vec2(700, 450), vec2(50, 500)));
	world.add_curve(std::make_shared<Arc>(vec2(1000, 700), 150, 0, M_PI));
	world.add_curve(std::make_shared<Segment>(vec2(850, 700), vec2(850, 500)));
	world.add_curve(std::make_shared<Segment>(vec2(1150, 700), vec2(1150, 500)));
	world.add_curve(std::make_shared<Arc>(vec2(1000, 500), 150, M_PI, 2*M_PI));
	world.add_curve(std::make_shared<Arc>(vec2(1000, 200), 100, 0, 2*M_PI));
	for (double angle : Globals::linspace(0, 2*M_PI, 1000)) {
		world.add_ball(std::make_shared<Ball>(vec2(250, 250), vec2(std::cos(angle), std::sin(angle))));
		world.add_ball(std::make_shared<Ball>(vec2(600, 250), vec2(std::cos(angle), std::sin(angle))));
		world.add_ball(std::make_shared<Ball>(vec2(420, 650), vec2(std::cos(angle), std::sin(angle))));
		world.add_ball(std::make_shared<Ball>(vec2(420, 820), vec2(std::cos(angle), std::sin(angle))));
		world.add_ball(std::make_shared<Ball>(vec2(1000, 600), vec2(std::cos(angle), std::sin(angle))));
		world.add_ball(std::make_shared<Ball>(vec2(1050, 200), vec2(std::cos(angle), std::sin(angle))));
	}
	save("world_debug.json", world);

	return 0;
}
