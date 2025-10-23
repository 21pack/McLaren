#pragma once
#include <SFML/System/Vector2.hpp>

namespace engine {

struct Camera {
	sf::Vector2f position; // Camera coordinates in the world
	float zoom = 1.0f;	   // TODO: needs?

	sf::Vector2f worldToScreen(sf::Vector2f worldPos)
		const; // Converts world coordinates to screen coordinates (isometry)

	sf::Vector2f
	screenToWorld(const sf::Vector2f &screenPos) const; // Inverse transformation

	void setTileSize(float w, float h) {
		tileWidth = w;
		tileHeight = h;
	}

  private:
	float tileWidth = 64.f;
	float tileHeight = 32.f;
};

} // namespace engine
