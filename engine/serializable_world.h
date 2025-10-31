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

struct  TileType {
	int tile_text_id;
	bool solid;
	template <class Archive>
	void serialize(Archive& ar) {
		ar(CEREAL_NVP(tile_text_id), CEREAL_NVP(solid));
	}
};

struct Area {
	sf::Rect<int> rect;
	TileType type{};
	template <class Archive>
	void serialize(Archive& ar) {
		auto [posX,posY] = rect.position;
		auto [sizeX, sizeY] = rect.size;
		ar(CEREAL_NVP(posX),CEREAL_NVP(posY), CEREAL_NVP(sizeX), CEREAL_NVP(sizeY), CEREAL_NVP(type));
	}

};

struct SerializableWorld {

	int height; // world height in tiles
	int width;
	std::unordered_map<int, TileTexture> textures;
	std::vector<Area> areas;
	template <class Archive>
	void serialize(Archive& ar) {
		ar(CEREAL_NVP(height), CEREAL_NVP(width),
			CEREAL_NVP(textures), CEREAL_NVP(areas));
	}
};

void to_json (const SerializableWorld& world, const std::string& filename);
SerializableWorld of_json (const std::string& filename);


} // namespace engine

