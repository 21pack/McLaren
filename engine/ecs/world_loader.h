#pragma once

#include "ecs/tile.h"
#include "resources/serializable_world.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace engine {

/**
 * @brief Loads a tile-based game world from a JSON file.
 *
 * Responsible for parsing a serialized world description, initializing the
 * tile layout and tile texture data. Converts JSON representation of world
 * areas into a flat tile vector for use by the engine.
 */
class WorldLoader {
  public:
	/**
	 * @brief Loads world data from a JSON file into provided containers.
	 * @param filename Path to the JSON file describing the world.
	 * @param width Output parameter for the width of the world in tiles.
	 * @param height Output parameter for the height of the world in tiles.
	 * @param tileTextures Output map of tile ID to `TileTexture` metadata.
	 * @param tiles Output vector of tiles representing the world layout.
	 */
	static void loadWorldFromJson(const std::string &filename, int &width,
								  int &height,
								  std::unordered_map<int, TileTexture> &tileTextures,
								  std::vector<Tile> &tiles);
};

} // namespace engine
