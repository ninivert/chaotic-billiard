#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
// #include <SFML/System.hpp>

#include <memory>  // std::make_shared
#include <cmath>
#include <iostream>
#include <fstream>

#include "argparse.hpp"
#include "json.hpp"

#include "from_json.hpp"

#include "world.hpp"
#include "ball.hpp"
#include "curve.hpp"
#include "vec2.hpp"

// unsigned int WINDOW_WIDTH(1200), WINDOW_HEIGHT(900);
unsigned int WINDOW_WIDTH(500), WINDOW_HEIGHT(500);

void draw(World const& world) {
	// clear the buffers
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	// draw balls
	glPointSize(1);
	glColor3f(0.0, 1.0, 0.5);
	glBegin(GL_POINTS);
	for (auto const& ball_ptr : world.ball_ptrs)
		glVertex2f(ball_ptr->pos.x/(WINDOW_WIDTH/2)-1, (ball_ptr->pos.y)/(WINDOW_HEIGHT/2)-1);
	glEnd();

	// draw curves
	glLineWidth(1);
	glColor3f(0.5, 0.5, 0.5);
	for (auto const& curve_ptr : world.curve_ptrs) {
		glBegin(GL_LINE_STRIP);
		vec2 pt;
		pt = (*curve_ptr)(0);
		glVertex2f(pt.x/(WINDOW_WIDTH/2)-1, pt.y/(WINDOW_HEIGHT/2)-1);
		for (double t : Globals::linspace(0, 1, 100)) {
			pt = (*curve_ptr)(t);
			glVertex2f(pt.x/(WINDOW_WIDTH/2)-1, pt.y/(WINDOW_HEIGHT/2)-1);
		}
		pt = (*curve_ptr)(1);
		glVertex2f(pt.x/(WINDOW_WIDTH/2)-1, pt.y/(WINDOW_HEIGHT/2)-1);
		glEnd();
	}
	glFlush();

}

int main(int argc, char const *argv[]) {
	argparse::ArgumentParser parser("chaotic billiard");

	parser.add_argument("worldfile")
		.help(".json file containing world information");

	parser.add_argument("--window")
		.help("display a render window")
		.default_value(false)
		.implicit_value(true);

	parser.add_argument("--render")
		.help("render to file (not recommended when --window is used)")
		.default_value(false)
		.implicit_value(true);

	parser.add_argument("--adaptative-dt")
		.help("use a flexible dt determined by framerate")
		.default_value(false)
		.implicit_value(true);

	parser.add_argument("--duration")
		.help("duration of the simulation")
		.scan<'g', double>()
		.default_value(100.0);

	parser.add_argument("--nsamples")
		.help("number of steps the simulation has to undergo")
		.scan<'i', int>()
		.default_value(100);

	parser.parse_args(argc, argv);

	// Parse world
	std::ifstream file(parser.get<std::string>("worldfile"));
	if (!file)
		throw std::runtime_error("failed to open file `" + parser.get<std::string>("worldfile") + "`");
	nlohmann::json j;
	file >> j;
	World world(World_from_json(j));
	// std::cout << world.str() << std::endl;

	if (parser.get<bool>("--window") || parser.get<bool>("--render")) {
		sf::RenderTexture texture;
		sf::Sprite sprite;
		texture.create(WINDOW_WIDTH, WINDOW_HEIGHT);

		unsigned int nsamples(parser.get<int>("--nsamples"));
		double duration(parser.get<double>("--duration"));
		double dt(duration/(nsamples-1));

		if (parser.get<bool>("--window")) {
			// Rendering with a window
			sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "chaotic billiard");
			sf::Clock clock;
			unsigned int frame_n(0);
			window.setVerticalSyncEnabled(true);

			while (window.isOpen()) {
				if (parser.get<bool>("--adaptative-dt")) {
					// TODO : framerate in window title
					double frame_dt(clock.restart().asSeconds());
					std::cout << frame_dt << std::endl;
					world.step(frame_dt*100);
				} else {
					world.step(dt);
				}

				if (!texture.setActive(true))
					std::cerr << "Failed to activate RenderTexture" << std::endl;
				draw(world);
				texture.display();
				sprite.setTexture(texture.getTexture());
				glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
				if (!texture.setActive(false))
					std::cerr << "Failed to deactivate RenderTexture" << std::endl;

				if (!window.setActive(true))
					std::cerr << "Failed to activate Window" << std::endl;
				window.clear();
				window.draw(sprite);
				window.display();
				if (!window.setActive(false))
					std::cerr << "Failed to deactivate Window" << std::endl;

				sf::Event event;
				if (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed)
						window.close();
					else if (event.type == sf::Event::Resized) {
						WINDOW_WIDTH = event.size.width;
						WINDOW_HEIGHT = event.size.height;
						glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
					}
				}

				if (parser.get<bool>("--render")) {
					texture.getTexture().copyToImage().saveToFile("frames/frame" + std::to_string(frame_n) + ".png");
				}

				++frame_n;
			}
		}

		else if (parser.get<bool>("--render")) {
			if (!texture.setActive(true))
				std::cerr << "Failed to activate RenderTexture" << std::endl;

			// Rendering without a window
			unsigned int nsamples(parser.get<int>("--nsamples"));
			double duration(parser.get<double>("--duration"));
			double dt(duration/(nsamples-1));
			double t(0);  // keep track of time to account for inaccuracies

			unsigned int frame_n(0);

			for (; frame_n < nsamples-1; ++frame_n) {
				t += dt;
				world.step(dt);

				draw(world);
				texture.display();
				texture.getTexture().copyToImage().saveToFile("frames/frame" + std::to_string(frame_n) + ".png");
				glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
			}
			world.step(duration-t);  // step the exact remaining time
			texture.clear();
			draw(world);
			texture.display();
			texture.getTexture().copyToImage().saveToFile("frames/frame" + std::to_string(frame_n) + ".png");

			if (!texture.setActive(false))
				std::cerr << "Failed to deactivate RenderTexture" << std::endl;
		}
	}

	return 0;
}
