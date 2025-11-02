#include "ecs/world_loader.h"
#include "resources/serializable_world.h"
#include "gtest/gtest.h"
#include <filesystem>

using namespace engine;

// --- Test loading basic world ---
TEST(WorldLoaderTest, LoadBasicWorld) {
	SerializableWorld world;
	world.world_width = 8;
	world.world_height = 8;
	world.textures = {{1, {"grass.png", 1}}, {2, {"stone.png", 1}}};

	// Area 1
	Area area1;
	area1.posX = 0;
	area1.posY = 0;
	area1.sizeX = 2;
	area1.sizeY = 2;
	area1.tile.layerIds = {1};
	area1.tile.solid = false;
	world.areas.push_back(area1);

	// Area 2
	Area area2;
	area2.posX = 2;
	area2.posY = 2;
	area2.sizeX = 2;
	area2.sizeY = 2;
	area2.tile.layerIds = {2};
	area2.tile.solid = true;
	world.areas.push_back(area2);

	std::string filename = "test_world.json";
	to_json(world, filename);

	int width = 0, height = 0;
	std::unordered_map<int, TileTexture> tileTextures;
	std::vector<Tile> tiles;

	WorldLoader::loadWorldFromJson(filename, width, height, tileTextures, tiles);

	// Check dimensions
	EXPECT_EQ(width, 8);
	EXPECT_EQ(height, 8);
	EXPECT_EQ(tiles.size(), 64u);

	// Check tileTextures loaded
	EXPECT_EQ(tileTextures.size(), 2u);
	EXPECT_TRUE(tileTextures.find(1) != tileTextures.end());
	EXPECT_TRUE(tileTextures.find(2) != tileTextures.end());

	// Check specific tiles
	auto getIndex = [&](int x, int y) {
		return (y + height / 2) * width + (x + width / 2);
	};

	// Area1 tiles
	for (int x = 0; x < 2; ++x) {
		for (int y = 0; y < 2; ++y) {
			const Tile &t = tiles[getIndex(x, y)];
			ASSERT_EQ(t.layerIds.size(), 1);
			EXPECT_EQ(t.layerIds[0], 1);
			EXPECT_FALSE(t.solid);
		}
	}

	// Area2 tiles
	for (int x = 2; x < 4; ++x) {
		for (int y = 2; y < 4; ++y) {
			const Tile &t = tiles[getIndex(x, y)];
			ASSERT_EQ(t.layerIds.size(), 1);
			EXPECT_EQ(t.layerIds[0], 2);
			EXPECT_TRUE(t.solid);
		}
	}

	// Cleanup
	std::filesystem::remove(filename);
}

// --- Test empty world (no areas) ---
TEST(WorldLoaderTest, LoadEmptyWorld) {
	SerializableWorld world;
	world.world_width = 1;
	world.world_height = 1;
	world.textures = {};

	std::string filename = "empty_world.json";
	to_json(world, filename);

	int width = 0, height = 0;
	std::unordered_map<int, TileTexture> tileTextures;
	std::vector<Tile> tiles;

	WorldLoader::loadWorldFromJson(filename, width, height, tileTextures, tiles);

	EXPECT_EQ(width, 1);
	EXPECT_EQ(height, 1);
	EXPECT_EQ(tiles.size(), 1u);
	EXPECT_TRUE(tileTextures.empty());

	// All tiles should be default-initialized
	for (const auto &t : tiles) {
		EXPECT_TRUE(t.layerIds.empty());
		EXPECT_FALSE(t.solid);
	}

	std::filesystem::remove(filename);
}
