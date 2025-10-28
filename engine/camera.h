#pragma once
#include <SFML/System/Vector2.hpp>

namespace engine {

class Camera {
  public:
	sf::Vector2f position; // Camera coordinates in the world
	float zoom = 2.0f;	   // TODO: needs?

	sf::Vector2f worldToScreen(sf::Vector2f worldPos)
		const; // Converts world coordinates to screen coordinates (isometry)

	sf::Vector2f
	screenToWorld(const sf::Vector2f &screenPos) const; // Inverse transformation

	void setTileSize(float w, float h) {
		tileWidth = w;
		tileHeight = h;
	}

	sf::Vector2f getTileSize() { return {tileWidth, tileHeight}; }

  private:
	float tileWidth = 32.f;
	float tileHeight = 32.f;
};

} // namespace engine
