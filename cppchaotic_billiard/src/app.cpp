#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>

#include <memory>  // std::make_shared
#include <cmath>
#include <iostream>

#include "world.hpp"
#include "ball.hpp"
#include "curve.hpp"
#include "vec2.hpp"

unsigned int constexpr WIDTH(1200);
unsigned int constexpr HEIGHT(900);

int main() {
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "chaotic billiard");
	window.setVerticalSyncEnabled(true);
	window.setActive(true);


	World world;
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

	for (double angle : linspace(0, 2*M_PI, 1000)) {
		world.add_ball(std::make_shared<Ball>(vec2(250, 250), vec2(std::cos(angle), std::sin(angle))));
		world.add_ball(std::make_shared<Ball>(vec2(600, 250), vec2(std::cos(angle), std::sin(angle))));
		world.add_ball(std::make_shared<Ball>(vec2(420, 650), vec2(std::cos(angle), std::sin(angle))));
		world.add_ball(std::make_shared<Ball>(vec2(420, 820), vec2(std::cos(angle), std::sin(angle))));
		world.add_ball(std::make_shared<Ball>(vec2(1000, 600), vec2(std::cos(angle), std::sin(angle))));
		world.add_ball(std::make_shared<Ball>(vec2(1050, 200), vec2(std::cos(angle), std::sin(angle))));
	}

	bool running(true);
	sf::Clock clock;

	while (running) {
		// handle events
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				// end the program
				running = false;
			}
			else if (event.type == sf::Event::Resized) {
				// adjust the viewport when the window is resized
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}

		double dt(clock.restart().asSeconds());
		std::cout << "dt = " << dt << std::endl;
		world.step(dt*100);

		// clear the buffers
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw balls
		glPointSize(1);
		glColor3f(0.0, 1.0, 0.5);
		glBegin(GL_POINTS);
		for (auto const& ball_ptr : world.ball_ptrs)
			glVertex2f(ball_ptr->pos.x/(WIDTH/2)-1, (ball_ptr->pos.y)/(HEIGHT/2)-1);
		glEnd();

		// draw curves
		glLineWidth(1);
		glColor3f(0.5, 0.5, 0.5);
		for (auto const& curve_ptr : world.curve_ptrs) {
			glBegin(GL_LINE_STRIP);
			vec2 pt;
			pt = (*curve_ptr)(0);
			glVertex2f(pt.x/(WIDTH/2)-1, pt.y/(HEIGHT/2)-1);
			for (double t : linspace(0, 1, 100)) {
				pt = (*curve_ptr)(t);
				glVertex2f(pt.x/(WIDTH/2)-1, pt.y/(HEIGHT/2)-1);
			}
			pt = (*curve_ptr)(1);
			glVertex2f(pt.x/(WIDTH/2)-1, pt.y/(HEIGHT/2)-1);
			glEnd();
		}
		glFlush();

		window.display();
	}

	return 0;
}
