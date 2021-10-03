#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "settings.hpp"
#include "exceptions.hpp"

class World;

class Renderer {
public:
	explicit Renderer() = default;
	virtual ~Renderer() = default;

	Renderer(Renderer const&) = delete;
	Renderer& operator = (Renderer const&) = delete;

	virtual void draw(World const& world) = 0;
};


class TextRenderer : public Renderer {
private:
	std::ostream * stream_ptr;
	std::ofstream ofs;

public:
	explicit TextRenderer(std::string const& filepath) {
		ofs = std::ofstream(filepath);
		if (ofs.fail()) ERROR(Exceptions::FILE_EXCEPTION);
		stream_ptr = &ofs;
	};
	explicit TextRenderer(std::ostream * stream_ptr = &std::cout)
		: stream_ptr(stream_ptr) {}

	virtual ~TextRenderer() {
		ofs.close();
	}

	TextRenderer(TextRenderer const&) = delete;
	TextRenderer& operator = (TextRenderer const&) = delete;

	void draw(World const& world) { *stream_ptr << world; }
};


class SFMLRenderer : public Renderer, public sf::RenderWindow {
public:
	explicit SFMLRenderer()
		: sf::RenderWindow(sf::VideoMode(Settings::WINDOW_WIDTH, Settings::WINDOW_HEIGHT), "chaotic billiard") {};
	virtual ~SFMLRenderer() {}

	SFMLRenderer(SFMLRenderer const&) = delete;
	SFMLRenderer& operator = (SFMLRenderer const&) = delete;

	void draw(World const& world) {
		// TODO
	}
};

#endif
