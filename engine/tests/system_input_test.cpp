#include "components.h"
#include "input.h"
#include "systems.h"
#include "gtest/gtest.h"
#include <cmath>
#include <entt/entt.hpp>
#include <vector>

const float TOLERANCE = 0.0001f;

// === Utility: create a player entity ===
inline entt::entity createPlayer(entt::registry &registry,
								 sf::Vector2f vel = {0.f, 0.f}, int animRow = 0) {
	auto entity = registry.create();
	registry.emplace<engine::Velocity>(entity, vel);
	registry.emplace<engine::PlayerControlled>(entity);
	registry.emplace<engine::Animation>(entity);
	registry.get<engine::Animation>(entity).row = animRow;
	return entity;
}

// === Utility: set keys pressed ===
inline void setKeys(engine::Input &input,
					const std::vector<sf::Keyboard::Key> &keys) {
	for (auto key : keys)
		input.keys[key] = true;
}

// === Utility: check velocity and animation row ===
inline void checkVelAndRow(const engine::Velocity &vel,
						   const engine::Animation &anim, float expectedX,
						   float expectedY, int expectedRow) {
	EXPECT_NEAR(vel.value.x, expectedX, TOLERANCE);
	EXPECT_NEAR(vel.value.y, expectedY, TOLERANCE);
	EXPECT_EQ(anim.row, expectedRow);
}

// --- No keys pressed ---
TEST(PlayerInputSystemTest, NoKeysPressed) {
	entt::registry registry;
	engine::Input input;
	auto player = createPlayer(registry);

	systems::playerInputSystem(registry, input);

	auto &vel = registry.get<engine::Velocity>(player);
	auto &anim = registry.get<engine::Animation>(player);

	checkVelAndRow(vel, anim, 0.f, 0.f, 0);
}

// --- Moving up ---
TEST(PlayerInputSystemTest, MoveUp) {
	entt::registry registry;
	engine::Input input;
	auto player = createPlayer(registry);

	setKeys(input, {sf::Keyboard::Key::W});

	systems::playerInputSystem(registry, input);

	auto &vel = registry.get<engine::Velocity>(player);
	auto &anim = registry.get<engine::Animation>(player);

	checkVelAndRow(vel, anim, 0.f, -1.f, 3); // up=3
}

// --- Moving down ---
TEST(PlayerInputSystemTest, MoveDown) {
	entt::registry registry;
	engine::Input input;
	auto player = createPlayer(registry);

	setKeys(input, {sf::Keyboard::Key::S});

	systems::playerInputSystem(registry, input);

	auto &vel = registry.get<engine::Velocity>(player);
	auto &anim = registry.get<engine::Animation>(player);

	checkVelAndRow(vel, anim, 0.f, 1.f, 0); // down=0
}

// --- Moving left ---
TEST(PlayerInputSystemTest, MoveLeft) {
	entt::registry registry;
	engine::Input input;
	auto player = createPlayer(registry);

	setKeys(input, {sf::Keyboard::Key::A});

	systems::playerInputSystem(registry, input);

	auto &vel = registry.get<engine::Velocity>(player);
	auto &anim = registry.get<engine::Animation>(player);

	checkVelAndRow(vel, anim, -1.f, 0.f, 2); // left=2
}

// --- Moving right ---
TEST(PlayerInputSystemTest, MoveRight) {
	entt::registry registry;
	engine::Input input;
	auto player = createPlayer(registry);

	setKeys(input, {sf::Keyboard::Key::D});

	systems::playerInputSystem(registry, input);

	auto &vel = registry.get<engine::Velocity>(player);
	auto &anim = registry.get<engine::Animation>(player);

	checkVelAndRow(vel, anim, 1.f, 0.f, 1); // right=1
}

// --- Moving up-right diagonal ---
TEST(PlayerInputSystemTest, MoveUpRightDiagonal) {
	entt::registry registry;
	engine::Input input;
	auto player = createPlayer(registry);

	setKeys(input, {sf::Keyboard::Key::W, sf::Keyboard::Key::D});

	systems::playerInputSystem(registry, input);

	auto &vel = registry.get<engine::Velocity>(player);
	auto &anim = registry.get<engine::Animation>(player);

	float len = std::sqrt(1.f + 1.f);
	checkVelAndRow(vel, anim, 1.f / len, -1.f / len, 3); // X dominates -> right=3
}

// --- Moving down-left diagonal --
TEST(PlayerInputSystemTest, MoveDownLeftDiagonal) {
	entt::registry registry;
	engine::Input input;
	auto player = createPlayer(registry);

	setKeys(input, {sf::Keyboard::Key::S, sf::Keyboard::Key::A});

	systems::playerInputSystem(registry, input);

	auto &vel = registry.get<engine::Velocity>(player);
	auto &anim = registry.get<engine::Animation>(player);

	float len = std::sqrt(1.f + 1.f);
	checkVelAndRow(vel, anim, -1.f / len, 1.f / len, 0); // Y dominates -> down=0
}

// --- Entity without PlayerControlled ---
TEST(PlayerInputSystemTest, OnlyVelocityAffectedPlayerControlled) {
	entt::registry registry;
	engine::Input input;

	auto entity = registry.create();
	registry.emplace<engine::Velocity>(entity, sf::Vector2f{0.f, 0.f});
	registry.emplace<engine::Animation>(entity);

	setKeys(input, {sf::Keyboard::Key::W, sf::Keyboard::Key::D});
	systems::playerInputSystem(registry, input);

	auto &vel = registry.get<engine::Velocity>(entity);
	auto &anim = registry.get<engine::Animation>(entity);

	checkVelAndRow(vel, anim, 0.f, 0.f, 0);
}
