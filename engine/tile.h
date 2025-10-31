#pragma once

#include <SFML/Graphics/Image.hpp>
#include <cereal/archives/json.hpp>

namespace engine {

struct Tile {
	std::vector<int> layerIds;
	bool solid = false;
	template <class Archive> void serialize(Archive &ar) {
		ar(cereal::make_nvp("textures_keys", layerIds), CEREAL_NVP(solid));
	}
};

struct TileData {
	sf::Image *image;
	int height;
};

} // namespace engine
