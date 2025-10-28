#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <map>

namespace engine {

struct Render;

class Input {
  public:
	std::map<sf::Keyboard::Key, bool> keys;

	bool
	pollEvents(Render &render); // Handles all window events (keyboard, mouse, close)

	bool
	isKeyDown(sf::Keyboard::Key key) const; // Check if a key is currently pressed
};

} // namespace engine
