#include "ecs/world_loader.h"

namespace engine {

void WorldLoader::loadWorldFromJson(
	const std::string &filename, int &width, int &height,
	std::unordered_map<int, TileTexture> &tileTextures, std::vector<Tile> &tiles) {
	SerializableWorld world = of_json(filename);
	height = world.world_height;
	width = world.world_width;
	tiles.resize(width * height);
	tileTextures = world.textures;

	auto getIndex = [&](int x, int y) {
		return (y + height / 2) * width + (x + width / 2);
	};

	for (int i = 0; i < world.areas.size(); i++) {
		auto a = world.areas[i];

		for (int x = a.posX; x < a.posX + a.sizeX; ++x) {
			for (int y = a.posY; y < a.posY + a.sizeY; ++y) {
				tiles[getIndex(x, y)] = a.tile;
			}
		}
	}
}

} // namespace engine
