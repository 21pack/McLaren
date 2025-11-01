#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace engine {

/**
 * @brief Camera class for handling isometric coordinate transformations and view
 * management.
 *
 * Provides functionality for converting between world coordinates and screen
 * coordinates in an isometric projection system. Manages camera position, zoom, and
 * view bounds.
 */
class Camera {
  public:
	sf::Vector2f size =
		sf::Vector2f(1000.f, 600.f); ///< Viewport size in world units
	sf::Vector2f position;			 ///< Camera center position in world coordinates
	float zoom = 2.0f;				 ///< Zoom level for scaling the view

	/**
	 * @brief Converts world coordinates to screen coordinates using isometric
	 * projection.
	 * @param worldPos World position to convert.
	 * @return Corresponding screen position.
	 */
	sf::Vector2f worldToScreen(sf::Vector2f worldPos) const;

	/**
	 * @brief Converts screen coordinates back to world coordinates using inverse
	 * isometric transformation.
	 * @param screenPos Screen position to convert.
	 * @return Corresponding world position.
	 */
	sf::Vector2f screenToWorld(const sf::Vector2f &screenPos) const;

	/**
	 * @brief Sets the dimensions of tiles in the isometric grid.
	 * @param w Tile width in world units.
	 * @param h Tile height in world units.
	 */
	void setTileSize(float w, float h) {
		tileWidth = w;
		tileHeight = h;
	}

	/**
	 * @brief Gets the camera's view bounds in world coordinates.
	 * @return FloatRect representing the visible area with margin applied.
	 */
	sf::FloatRect getBounds() const {
		return sf::FloatRect({(position.x - size.x / 2.f + margin),
							  (position.y - size.y / 2.f + margin)},
							 {(size.x + margin), (size.y + margin)});
	}

	/**
	 * @brief Gets the current tile dimensions.
	 * @return Vector2f containing tile width and height.
	 */
	sf::Vector2f getTileSize() const { return {tileWidth, tileHeight}; }

  private:
	float tileWidth = 32.f;	 ///< Width of each tile in world units
	float tileHeight = 32.f; ///< Height of each tile in world units
	float margin = 16.f;	 ///< Margin around camera bounds for culling purposes
};

} // namespace engine
