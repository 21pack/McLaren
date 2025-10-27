#pragma once

namespace engine {

struct Tile {
	std::vector<int> layerIds;
	bool solid = false;
};

} // namespace engine
