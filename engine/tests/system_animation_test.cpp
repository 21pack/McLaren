#include "components.h"
#include "input.h"
#include "systems.h"
#include "gtest/gtest.h"
#include <entt/entt.hpp>

const float TOLERANCE = 0.0001f;

TEST(SystemsTest, AnimationSystem) {
	entt::registry registry;
	auto entity = registry.create();

	auto &anim = registry.emplace<engine::Animation>(entity);

	engine::AnimationClip idleClip;
	idleClip.frameCount = 4;
	idleClip.frameDuration = 0.1f;

	// anim.clips[engine::AnimationState::Idle] = idleClip;

	// anim.state = engine::AnimationState::Idle;
	anim.frameIdx = 0;
	anim.frameTime = 0.0f;

	// run with dt < time to flip frame
	systems::animationSystem(registry, 0.05f);
	EXPECT_EQ(anim.frameIdx, 0);
	EXPECT_NEAR(anim.frameTime, 0.05f, TOLERANCE);

	// run again to flip frame
	systems::animationSystem(registry, 0.06f);
	EXPECT_EQ(anim.frameIdx, 1);
	EXPECT_NEAR(anim.frameTime, 0.01f, TOLERANCE);
}
