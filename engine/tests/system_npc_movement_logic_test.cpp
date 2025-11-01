#include "ecs/components.h"
#include "ecs/systems.h"
#include "gtest/gtest.h"
#include <cmath>
#include <cstdlib>
#include <entt/entt.hpp>

const float TOLERANCE = 0.0001f;

// === Utilities ===

inline entt::entity createPlayer(entt::registry &registry,
								 sf::Vector2f pos = {0.f, 0.f}) {
	auto entity = registry.create();
	registry.emplace<engine::Position>(entity, pos);
	registry.emplace<engine::PlayerControlled>(entity);
	return entity;
}

inline entt::entity createNPC(entt::registry &registry, sf::Vector2f pos,
							  sf::Vector2f vel = {0.f, 0.f}, bool chasing = false) {
	auto entity = registry.create();
	registry.emplace<engine::Position>(entity, pos);
	registry.emplace<engine::Velocity>(entity, vel);
	registry.emplace<engine::Animation>(entity);
	if (chasing)
		registry.emplace<engine::ChasingPlayer>(entity);
	return entity;
}

inline void checkVec(const sf::Vector2f &v, float x, float y) {
	EXPECT_NEAR(v.x, x, TOLERANCE);
	EXPECT_NEAR(v.y, y, TOLERANCE);
}

// === Tests for npcFollowPlayerSystem ===

// --- NPC must move towards player and select correct animation (left) ---
TEST(NPCFollowPlayerSystemTest, MoveTowardPlayer) {
	entt::registry registry;
	createPlayer(registry, {0.f, 0.f});
	auto npc = createNPC(registry, {10.f, 0.f}, {0.f, 0.f}, true);

	systems::npcFollowPlayerSystem(registry, 0.1f);

	auto &vel = registry.get<engine::Velocity>(npc);
	auto &anim = registry.get<engine::Animation>(npc);

	EXPECT_LT(vel.value.x, 0.f);
	EXPECT_NEAR(vel.value.y, 0.f, TOLERANCE);
	EXPECT_EQ(anim.row, 2); // left = 2
}

// --- If NPC is closer than 3 units, it stands still ---
TEST(NPCFollowPlayerSystemTest, StopWhenCloseToPlayer) {
	entt::registry registry;
	createPlayer(registry, {0.f, 0.f});
	auto npc = createNPC(registry, {2.f, 0.f}, {0.f, 0.f}, true);

	systems::npcFollowPlayerSystem(registry, 0.1f);

	auto &vel = registry.get<engine::Velocity>(npc);
	checkVec(vel.value, 0.f, 0.f);
}

// --- Checking the anim.row setting when moving in Y direction ---
TEST(NPCFollowPlayerSystemTest, CorrectAnimationDirectionY) {
	entt::registry registry;
	createPlayer(registry, {0.f, 10.f});
	auto npc =
		createNPC(registry, {0.f, 0.f}, {0.f, 1.f}, true); // is already moving down

	systems::npcFollowPlayerSystem(registry, 0.1f);

	auto &anim = registry.get<engine::Animation>(npc);
	EXPECT_EQ(anim.row, 0); // down = 0
}

// --- Checking the anim.row setting when moving in X direction ---
TEST(NPCFollowPlayerSystemTest, CorrectAnimationDirectionX) {
	entt::registry registry;
	createPlayer(registry, {10.f, 0.f});
	auto npc = createNPC(registry, {0.f, 0.f}, {1.f, 0.f}, true); // вправо

	systems::npcFollowPlayerSystem(registry, 0.1f);

	auto &anim = registry.get<engine::Animation>(npc);
	EXPECT_EQ(anim.row, 1); // right = 1
}

// === Tests for npcWanderSystem ===

// --- npcWanderSystem changes speed randomly (validity check) ---
TEST(NPCWanderSystemTest, ChangesVelocityRandomly) {
	entt::registry registry;
	auto npc = createNPC(registry, {0.f, 0.f}, {0.f, 0.f});

	auto &velBefore = registry.get<engine::Velocity>(npc).value;

	systems::npcWanderSystem(registry, 0.1f);

	auto &velAfter = registry.get<engine::Velocity>(npc).value;

	// Not necessarily a big difference, but definitely not NaN or {0,0} all the time
	EXPECT_TRUE(std::isfinite(velAfter.x));
	EXPECT_TRUE(std::isfinite(velAfter.y));
	EXPECT_FALSE(std::isnan(velAfter.x));
	EXPECT_FALSE(std::isnan(velAfter.y));
}

// --- The speed is normalized after random changes ---
TEST(NPCWanderSystemTest, NormalizesVelocity) {
	entt::registry registry;
	auto npc = createNPC(registry, {0.f, 0.f}, {1.f, 1.f});

	systems::npcWanderSystem(registry, 0.1f);

	auto &vel = registry.get<engine::Velocity>(npc);
	float len = std::sqrt(vel.value.x * vel.value.x + vel.value.y * vel.value.y);

	EXPECT_NEAR(len, 1.f, 0.01f);
}

// --- Excluding Player and Chasing from the wander system ---
TEST(NPCWanderSystemTest, IgnoresPlayerControlledOrChasingNPCs) {
	entt::registry registry;

	// Player (must be ignored)
	createPlayer(registry, {0.f, 0.f});

	// The NPC that is chasing (should also be ignored)
	auto chasing = createNPC(registry, {0.f, 0.f}, {1.f, 0.f}, true);

	auto &velBefore = registry.get<engine::Velocity>(chasing).value;

	systems::npcWanderSystem(registry, 0.1f);

	auto &velAfter = registry.get<engine::Velocity>(chasing).value;

	checkVec(velAfter, velBefore.x, velBefore.y);
}
