#include "components.h"
#include "input.h"
#include "systems.h"
#include "gtest/gtest.h"
#include <entt/entt.hpp>

const float TOLERANCE = 0.0001f;

TEST(SystemsTest, MovementSystem) {
	entt::registry registry;
	const float dt = 0.5f;
	const float speed = 5.f;

	auto entity = registry.create();
	registry.emplace<engine::Position>(entity, sf::Vector2f{10.f, 20.f});
	registry.emplace<engine::Velocity>(entity, sf::Vector2f{1.f, 0.f});
	registry.emplace<engine::Speed>(entity, speed);

	systems::movementSystem(registry, dt);

	const auto &pos = registry.get<engine::Position>(entity);

	// pos.x = 10.f + (vel.x * speed * dt) = 10 + (1 * 5 * 0.5) = 12.5
	// pos.y = 20.f + (vel.y * speed * dt) = 20 + (0 * 5 * 0.5) = 20.0
	EXPECT_NEAR(pos.value.x, 12.5f, TOLERANCE);
	EXPECT_NEAR(pos.value.y, 20.0f, TOLERANCE);
}
