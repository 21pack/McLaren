#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <map>

namespace engine {

struct Renderer;

struct Input {
	std::map<sf::Keyboard::Key, bool> keys;

	void pollEvents(
		Renderer &renderer); // Handles all window events (keyboard, mouse, close)

	bool
	isKeyDown(sf::Keyboard::Key key) const; // Сheck if a key is currently pressed
};

} // namespace engine
