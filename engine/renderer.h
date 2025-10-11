#pragma once
#include <SFML/Graphics.hpp>

namespace engine {

// Не очень уверен, что функции в структуре правильные, мб надо что-то
// добавить/убрать

struct Renderer {
	sf::RenderWindow window;

	Renderer(unsigned width = 1920u, unsigned height = 1080u,
			 const char *title = "Game")
		: window(sf::VideoMode({width, height}), title) {}

	bool isOpen() const { return window.isOpen(); }
	void clear(const sf::Color &color = sf::Color::Black) { window.clear(color); }
	void present() { window.display(); }
	void draw(const sf::Drawable &drawable) { window.draw(drawable); }

	sf::RenderWindow &getWindow() { return window; }
	void closeWindow() { window.close(); }
};

} // namespace engine
