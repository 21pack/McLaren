#pragma once
#include "camera.h"
#include "loop.h"
#include "render_frame.h"
#include <SFML/Graphics.hpp>
#include <mutex>

namespace engine {

struct Render {
	sf::RenderWindow window;

	Render(unsigned width = 1920u, unsigned height = 1080u,
		   const char *title = "Game")
		: window(sf::VideoMode({width, height}), title) {}

	bool isOpen() const { return window.isOpen(); }
	void clear(const sf::Color &color = sf::Color::Black) { window.clear(color); }
	void present() { window.display(); }

	std::shared_ptr<RenderFrame> collectFrame(ILoop &loop, Camera &camera);
	void drawFrame(const RenderFrame &frame);

	sf::RenderWindow &getWindow() { return window; }
	void closeWindow() { window.close(); }
};

struct RenderQueue {
	std::shared_ptr<RenderFrame> frontFrame; // for draw
	std::shared_ptr<RenderFrame> backFrame;	 // for render

	bool updated = false;
	std::mutex mtx;

	RenderQueue() {
		frontFrame = std::make_shared<RenderFrame>();
		backFrame = std::make_shared<RenderFrame>();
	}

	void swap() { std::swap(frontFrame, backFrame); }
};

} // namespace engine
