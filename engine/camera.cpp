#include "camera.h"
#include <limits>
#include <cmath>

namespace engine {

const float TILE_WIDTH_HALF = 32.f;
const float TILE_HEIGHT_HALF = 16.f;

sf::Vector2f Camera::worldToScreen(sf::Vector2f worldPos) const {
    float screenX = (worldPos.x - worldPos.y) * TILE_WIDTH_HALF;
    float screenY = (worldPos.x + worldPos.y) * TILE_HEIGHT_HALF;

    return {
        (screenX * zoom) + position.x,
        (screenY * zoom) + position.y
    };
}

sf::Vector2f Camera::screenToWorld(const sf::Vector2f &screenPos) const {
    if (std::abs(zoom) < std::numeric_limits<float>::epsilon()) {
        return {0.f, 0.f};
    }
    
    float screenX = (screenPos.x - position.x) / zoom;
    float screenY = (screenPos.y - position.y) / zoom;

    float worldX = (screenX / TILE_WIDTH_HALF + screenY / TILE_HEIGHT_HALF) / 2.0f;
    float worldY = (screenY / TILE_HEIGHT_HALF - screenX / TILE_WIDTH_HALF) / 2.0f;

    return {worldX, worldY};
}

} // namespace engine