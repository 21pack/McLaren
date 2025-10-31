#include "components.h"
#include "input.h"
#include "systems.h"
#include "gtest/gtest.h"
#include <entt/entt.hpp>

// === Formulas for tests ===
// pos.x = 10.f + (vel.x * speed * dt)
// pos.y = 20.f + (vel.y * speed * dt)

const float TOLERANCE = 0.0001f;
const int DEFAULT_WIDTH = 20;
const int DEFAULT_HEIGHT = 20;
const float DEFAULT_SPEED = 5.f;
const float DEFAULT_DT = 0.5f;

// === Utility: create a world of tiles ===
inline std::vector<engine::Tile> createTiles(int width = DEFAULT_WIDTH,
											 int height = DEFAULT_HEIGHT,
											 bool solid = false) {
	std::vector<engine::Tile> tiles(width * height);
	for (auto &tile : tiles)
		tile.solid = solid;
	return tiles;
}

// === Utility: create an entity with movement components ===
inline entt::entity createEntity(entt::registry &registry, sf::Vector2f position,
								 sf::Vector2f velocity = {0.f, 0.f},
								 float speed = DEFAULT_SPEED) {
	auto entity = registry.create();
	registry.emplace<engine::Position>(entity, position);
	registry.emplace<engine::Velocity>(entity, velocity);
	registry.emplace<engine::Speed>(entity, speed);
	return entity;
}

// === Helper: run movement system ===
inline void move(entt::registry &registry, std::vector<engine::Tile> &tiles,
				 int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT,
				 float dt = DEFAULT_DT) {
	systems::movementSystem(registry, tiles, width, height, dt);
}

// --- Movement without obstacles ---
TEST(SystemsTest, MoveFree) {
	entt::registry registry;
	auto tiles = createTiles();
	auto entity = createEntity(registry, {10.f, 10.f}, {1.f, 0.f});

	move(registry, tiles);

	const auto &pos = registry.get<engine::Position>(entity);
	EXPECT_NEAR(pos.value.x, 12.5f, TOLERANCE);
	EXPECT_NEAR(pos.value.y, 10.0f, TOLERANCE);
}

// --- Movement blocked by a solid tile ---
TEST(SystemsTest, BlockedByTile) {
	entt::registry registry;
	auto tiles = createTiles();
	tiles[10 * DEFAULT_WIDTH + 11].solid = true;

	auto entity = createEntity(registry, {10.f, 10.f}, {1.f, 0.f});

	move(registry, tiles);

	const auto &pos = registry.get<engine::Position>(entity);
	EXPECT_NEAR(pos.value.x, 10.f, TOLERANCE);
	EXPECT_NEAR(pos.value.y, 10.f, TOLERANCE);
}

// --- Diagonal sliding motion ---
TEST(SystemsTest, SlideAlongWall) {
	entt::registry registry;
	auto tiles = createTiles();
	tiles[10 * DEFAULT_WIDTH + 11].solid = true;

	auto entity = createEntity(registry, {10.f, 10.f}, {1.f, 1.f});

	move(registry, tiles);

	const auto &pos = registry.get<engine::Position>(entity);
	EXPECT_NEAR(pos.value.x, 10.f, TOLERANCE);
	EXPECT_NEAR(pos.value.y, 12.5f, TOLERANCE);
}

// --- Limitation by the boundaries of the world ---
TEST(SystemsTest, StayWithinWorld) {
	entt::registry registry;
	auto tiles = createTiles();

	auto entity = createEntity(registry, {19.f, 19.f}, {1.f, 1.f});

	move(registry, tiles);

	const auto &pos = registry.get<engine::Position>(entity);
	EXPECT_NEAR(pos.value.x, 19.f, TOLERANCE);
	EXPECT_NEAR(pos.value.y, 19.f, TOLERANCE);
}
