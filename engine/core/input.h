#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <map>

namespace engine {

struct Render;

/**
 * @brief Handles input events and keyboard state management.
 *
 * Provides interface for polling window events and checking keyboard state.
 * Manages key press states and window event processing.
 */
class Input {
  public:
	std::map<sf::Keyboard::Key, bool>
		keys; ///< Current state of keyboard keys (true = pressed)

	/**
	 * @brief Processes all pending window events.
	 * @param render Reference to the Render system for window access.
	 * @return True if the application should continue running, false if should exit.
	 *
	 * Handles keyboard events, mouse events, and window close requests.
	 * Updates the internal key state map based on keyboard events.
	 */
	bool pollEvents(Render &render);

	/**
	 * @brief Checks if a specific key is currently pressed down.
	 * @param key The keyboard key to check.
	 * @return True if the key is pressed, false otherwise.
	 */
	bool isKeyDown(sf::Keyboard::Key key) const;
};

} // namespace engine
