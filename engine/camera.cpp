#include "camera.h"

#include <cmath>
#include <limits>

namespace engine {

sf::Vector2f Camera::worldToScreen(sf::Vector2f worldPos) const {
	const float tileWidthHalf = tileWidth * 0.5f;
	const float tileHeightHalf = tileHeight * 0.5f;

	float screenX = (worldPos.x - worldPos.y) * tileWidthHalf;
	float screenY = (worldPos.x + worldPos.y) * tileHeightHalf;

	return {(screenX * zoom), (screenY * zoom)};
}

sf::Vector2f Camera::screenToWorld(const sf::Vector2f &screenPos) const {
	if (std::abs(zoom) < std::numeric_limits<float>::epsilon()) {
		return {0.f, 0.f};
	}

	const float tileWidthHalf = tileWidth * 0.5f;
	const float tileHeightHalf = tileHeight * 0.5f;

	float screenX = (screenPos.x) / zoom;
	float screenY = (screenPos.y) / zoom;

	float worldX = (screenX / tileWidthHalf + screenY / tileHeightHalf) / 2.0f;
	float worldY = (screenY / tileHeightHalf - screenX / tileWidthHalf) / 2.0f;

	return {worldX, worldY};
}

} // namespace engine
