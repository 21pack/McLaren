#include "core/input.h"

#include "core/render.h"

namespace engine {

bool Input::pollEvents(Render &render) {
	while (auto event = render.getWindow().pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			return true;
		} else if (auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			if (keyPressed->code != sf::Keyboard::Key::Unknown) {
				keys[keyPressed->code] = true;
			}
		} else if (auto *keyReleased = event->getIf<sf::Event::KeyReleased>()) {
			if (keyReleased->code != sf::Keyboard::Key::Unknown) {
				keys[keyReleased->code] = false;
			}
		}
	}
	return false;
}

bool Input::isKeyDown(sf::Keyboard::Key key) const {
	if (key == sf::Keyboard::Key::Unknown) {
		return false;
	}
	auto it = keys.find(key);
	if (it != keys.end()) {
		return it->second;
	}
	return false;
}

} // namespace engine
