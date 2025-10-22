#include "input.h"
#include "render.h"

namespace engine {

void Input::pollEvents(Render &render) {
	while (auto event = render.getWindow().pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			render.closeWindow();
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
