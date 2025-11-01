#include "ecs/components.h"
#include "ecs/systems.h"
#include "gtest/gtest.h"
#include <entt/entt.hpp>

const float TOLERANCE = 0.0001f;

// === Utility: create an entity with animation ===
inline entt::entity createAnimatedEntity(entt::registry &registry, int state = 0,
										 int frameCount = 1,
										 float frameDuration = 0.1f,
										 int startFrame = 0,
										 float startTime = 0.0f) {
	auto entity = registry.create();
	auto &anim = registry.emplace<engine::Animation>(entity);
	engine::AnimationClip clip;
	clip.frameCount = frameCount;
	clip.frameDuration = frameDuration;
	anim.clips[state] = clip;
	anim.state = state;
	anim.frameIdx = startFrame;
	anim.frameTime = startTime;
	return entity;
}

// === Utility: check frame index and frame time ===
inline void checkAnimation(const engine::Animation &anim, int expectedIdx,
						   float expectedTime) {
	EXPECT_EQ(anim.frameIdx, expectedIdx);
	EXPECT_NEAR(anim.frameTime, expectedTime, TOLERANCE);
}

// --- Basic test animation ---
TEST(AnimationSystemTest, BasicAdvance) {
	entt::registry registry;
	auto entity = createAnimatedEntity(registry, 0, 4, 0.1f, 0, 0.0f);
	auto &anim = registry.get<engine::Animation>(entity);

	// run with dt < time to flip frame
	systems::animationSystem(registry, 0.05f);
	checkAnimation(anim, 0, 0.05f);

	// dt > frameDuration, run again to flip frame
	systems::animationSystem(registry, 0.06f); // 0.05 + 0.06 = 0.11 > 0.1
	checkAnimation(anim, 1, 0.01f);			   // remainder 0.01
}

// --- Checking frame looping ---
TEST(AnimationSystemTest, LoopingFrames) {
	entt::registry registry;
	auto entity = createAnimatedEntity(registry, 0, 3, 0.1f, 2, 0.05f);
	auto &anim = registry.get<engine::Animation>(entity);

	systems::animationSystem(registry, 0.1f); // 0.05 + 0.1 = 0.15
	checkAnimation(anim, 0, 0.05f);			  // loop
}

// --- Checking frameCount = 1 (should not change) ---
TEST(AnimationSystemTest, SingleFrameClip) {
	entt::registry registry;
	auto entity = createAnimatedEntity(registry, 0, 1, 0.1f, 0, 0.0f);
	auto &anim = registry.get<engine::Animation>(entity);

	systems::animationSystem(registry, 0.5f); // should not change
	checkAnimation(anim, 0, 0.0f);
}

// --- Checking if a clip is missing for the state ---
TEST(AnimationSystemTest, MissingClip) {
	entt::registry registry;
	auto entity = registry.create();
	auto &anim = registry.emplace<engine::Animation>(entity);
	anim.state = 1; // there is no clip with this key.
	anim.frameIdx = 0;
	anim.frameTime = 0.0f;

	systems::animationSystem(registry, 0.2f);
	checkAnimation(anim, 0, 0.0f); // nothing has changed
}

// --- Checking time accumulation (multiple frames per call) ---
TEST(AnimationSystemTest, MultipleFrameAdvance) {
	entt::registry registry;
	auto entity = createAnimatedEntity(registry, 0, 4, 0.1f, 0, 0.0f);
	auto &anim = registry.get<engine::Animation>(entity);

	systems::animationSystem(registry, 0.35f); // 3 frames must pass: 0->3
	checkAnimation(anim, 3, 0.05f);			   // remaining time
}

// --- Checking multiple entities ---
TEST(AnimationSystemTest, MultipleEntities) {
	entt::registry registry;

	auto e1 = createAnimatedEntity(registry, 0, 3, 0.1f, 0, 0.05f);
	auto e2 = createAnimatedEntity(registry, 0, 2, 0.2f, 1, 0.1f);

	auto &anim1 = registry.get<engine::Animation>(e1);
	auto &anim2 = registry.get<engine::Animation>(e2);

	systems::animationSystem(registry, 0.1f);

	checkAnimation(anim1, 1, 0.05f); // advanced by 1 frame
	checkAnimation(anim2, 0, 0.0f);	 // loop
}
