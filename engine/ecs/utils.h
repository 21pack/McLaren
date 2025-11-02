#pragma once

#include "ecs/tile.h"
#include "resources/image_manager.h"
#include "resources/serializable_world.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <unordered_map>

namespace engine {

/**
 * @brief Calculates the bounding rectangle of non-transparent content within a
 * frame.
 * @param image Source image to analyze.
 * @param frameRect The frame rectangle within the image to check.
 * @return IntRect representing the smallest rectangle containing non-transparent
 * pixels.
 *
 * Scans the specified frame area and finds the minimal rectangle that contains
 * all pixels with alpha value above the threshold.
 */
inline sf::IntRect calculateContentRect(const sf::Image &image,
										sf::IntRect frameRect) {
	int minX = frameRect.size.x;
	int minY = frameRect.size.y;
	int maxX = -1;
	int maxY = -1;

	int frameLeft = frameRect.position.x;
	int frameTop = frameRect.position.y;

	int startX = std::max(frameLeft, 0);
	int startY = std::max(frameTop, 0);
	int endX =
		std::min(frameLeft + frameRect.size.x, static_cast<int>(image.getSize().x));
	int endY =
		std::min(frameTop + frameRect.size.y, static_cast<int>(image.getSize().y));

	const char ALPHA_THRESHOLD = 10;

	for (int y = startY; y < endY; ++y) {
		for (int x = startX; x < endX; ++x) {
			if (image.getPixel({(unsigned int)x, (unsigned int)y}).a >
				ALPHA_THRESHOLD) {
				int relativeX = x - frameLeft;
				int relativeY = y - frameTop;

				minX = std::min(minX, relativeX);
				minY = std::min(minY, relativeY);
				maxX = std::max(maxX, relativeX);
				maxY = std::max(maxY, relativeY);
			}
		}
	}

	if (maxX < minX) {
		return {{0, 0}, {0, 0}};
	}

	return {{minX, minY}, {(maxX - minX) + 1, (maxY - minY) + 1}};
}

/**
 * @brief Generates a mapping from tile IDs to their corresponding visual data.
 * @param textures A map of tile IDs to their texture metadata (`TileTexture`).
 * @param imgMgr Reference to the `ImageManager` used to load or retrieve textures.
 * @return An unordered_map where each key is a tile ID and the value is a `TileData`
 *         struct containing a pointer to the image and the tile's height.
 */
inline std::unordered_map<int, TileData>
makeTileData(const std::unordered_map<int, TileTexture> &textures,
			 ImageManager &imgMgr) {
	std::unordered_map<int, TileData> tileImages;

	for (auto keyvalue : textures) {
		int key = keyvalue.first;
		auto tex = keyvalue.second;
		tileImages[key] = {&imgMgr.getImage(tex.texture_src), tex.height};
	}

	return tileImages;
}

} // namespace engine
