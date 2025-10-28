#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace engine {

class Camera {
  public:
	sf::Vector2f size = sf::Vector2f(1000.f, 600.f);
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

	sf::FloatRect getBounds() const {
		return sf::FloatRect({(position.x - size.x / 2.f + margin),
							  (position.y - size.y / 2.f + margin)},
							 {(size.x + margin), (size.y + margin)});
	}

	sf::Vector2f getTileSize() const { return {tileWidth, tileHeight}; }

  private:
	float tileWidth = 32.f;
	float tileHeight = 32.f;
	float margin = 16.f;
};

} // namespace engine
