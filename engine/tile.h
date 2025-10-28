#pragma once

#include <SFML/Graphics/Image.hpp>

namespace engine {

struct Tile {
	std::vector<int> layerIds;
	bool solid = false;
};

struct TileData {
	sf::Image *image;
	int height;
};

} // namespace engine
