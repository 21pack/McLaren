#include "components.h"
#include "input.h"
#include "systems.h"
#include "gtest/gtest.h"
#include <entt/entt.hpp>

const float TOLERANCE = 0.0001f;

TEST(SystemsTest, PlayerInputSystem) {
	entt::registry registry;
	engine::Input input;

	auto entity = registry.create();
	registry.emplace<engine::Velocity>(entity, sf::Vector2f{0.f, 0.f});
	registry.emplace<engine::PlayerControlled>(entity);

	input.keys[sf::Keyboard::Key::W] = true;
	input.keys[sf::Keyboard::Key::D] = true;

	systems::playerInputSystem(registry, input);

	const auto &vel = registry.get<engine::Velocity>(entity);

	float len = std::sqrt(1.f * 1.f + (-1.f) * (-1.f));
	EXPECT_NEAR(vel.value.x, 1.f / len, TOLERANCE);
	EXPECT_NEAR(vel.value.y, -1.f / len, TOLERANCE);
}
