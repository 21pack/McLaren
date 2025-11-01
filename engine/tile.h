#pragma once

#include <SFML/Graphics/Image.hpp>
#include <cereal/archives/json.hpp>

namespace engine {

/**
 * @brief Represents a tile in the game world with layer and collision data.
 */
struct Tile {
	std::vector<int> layerIds; ///< Texture IDs for each layer of the tile
	bool solid = false;		   ///< Whether the tile blocks movement
	template <class Archive> void serialize(Archive &ar) {
		ar(cereal::make_nvp("textures_keys", layerIds), CEREAL_NVP(solid));
	}
};

/**
 * @brief Contains visual data for a tile type.
 */
struct TileData {
	sf::Image *image; ///< Pointer to the tile texture image
	int height;		  ///< Height of the tile for rendering
};

} // namespace engine
