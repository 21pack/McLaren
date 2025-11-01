#pragma once

#include <cereal/archives/json.hpp>
#include <string>
#include <tile.h>
#include <unordered_map>

namespace engine {

/**
 * @brief Defines texture data for tiles with serialization support.
 */
struct TileTexture {
	std::string texture_src; ///< Path to the texture file
	int height;				 ///< Height of the tile texture
	template <class Archive> void serialize(Archive &ar) {
		ar(CEREAL_NVP(texture_src), CEREAL_NVP(height));
	}
};

/**
 * @brief Represents a rectangular area in the world with uniform tile type.
 */
struct Area {
	int posX;  ///< X coordinate of the area's top-left corner
	int posY;  ///< Y coordinate of the area's top-left corner
	int sizeX; ///< Width of the area in tiles
	int sizeY; ///< Height of the area in tiles
	Tile tile; ///< Tile type for this area
	template <class Archive> void serialize(Archive &ar) {
		ar(CEREAL_NVP(posX), CEREAL_NVP(posY), CEREAL_NVP(sizeX), CEREAL_NVP(sizeY),
		   CEREAL_NVP(tile));
	}
};

/**
 * @brief Serializable container for world data including dimensions, textures, and
 * areas.
 */
struct SerializableWorld {
	int world_height; ///< World height in tiles
	int world_width;  ///< World width in tiles
	std::unordered_map<int, TileTexture>
		textures;			 ///< Map of tile IDs to texture data
	std::vector<Area> areas; ///< Collection of defined areas in the world
	template <class Archive>
	void serialize(Archive &ar, const std::uint32_t version) {
		ar(CEREAL_NVP(world_height), CEREAL_NVP(world_width), CEREAL_NVP(textures),
		   CEREAL_NVP(areas));
	}
};

/**
 * @brief Serializes a world object to JSON file.
 * @param world World data to serialize.
 * @param filename Output file path for JSON data.
 */
void to_json(const SerializableWorld &world, const std::string &filename);

/**
 * @brief Deserializes a world object from JSON file.
 * @param filename Input file path containing JSON world data.
 * @return Deserialized SerializableWorld object.
 */
SerializableWorld of_json(const std::string &filename);

} // namespace engine
