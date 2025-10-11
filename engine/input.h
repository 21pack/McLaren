#pragma once
#include <SFML/Window/Event.hpp>

namespace engine {

struct Input {
	bool keys[sf::Keyboard::KeyCount]{};

	void pollEvents(
		Renderer &renderer); // Handles all window events (keyboard, mouse, close)

	bool
	isKeyDown(sf::Keyboard::Key key) const; // Сheck if a key is currently pressed
};

} // namespace engine
