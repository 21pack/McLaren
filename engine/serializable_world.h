#pragma once

#include <string>
#include <tile.h>
#include <unordered_map>
#include <cereal/archives/json.hpp>


namespace engine {
struct TileTexture
{
	std::string texture_src; // path to texture file
	int height;
	template <class Archive>
	void serialize(Archive& ar) {
		ar(CEREAL_NVP(texture_src), CEREAL_NVP(height));
	}
};

struct Area {
	int posX;
	int posY;
	int sizeX;
	int sizeY;
	Tile tile;
	template <class Archive>
	void serialize(Archive& ar) {
		ar(CEREAL_NVP(posX),CEREAL_NVP(posY), CEREAL_NVP(sizeX), CEREAL_NVP(sizeY), CEREAL_NVP(tile));
	}

};

struct SerializableWorld {

	int world_height; // world height in tiles
	int world_width;
	std::unordered_map<int, TileTexture> textures;
	std::vector<Area> areas;
	template <class Archive>
	void serialize(Archive& ar, const std::uint32_t version) {
		ar(CEREAL_NVP(world_height), CEREAL_NVP(world_width),
			CEREAL_NVP(textures), CEREAL_NVP(areas));
	}
};

void to_json (const SerializableWorld& world, const std::string& filename);
SerializableWorld of_json (const std::string& filename);


} // namespace engine

