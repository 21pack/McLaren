#include "components.h"
#include "engine.h"
#include "systems.h"
#include "gtest/gtest.h"
#include <entt/entt.hpp>
#include <unordered_map>

const float TOLERANCE = 0.0001f;

// === Utility: creating test animation clips ===
static std::unordered_map<int, engine::AnimationClip> makeTestClips() {
	engine::AnimationClip clip;
	clip.texture = "wolf.png";
	clip.frameCount = 4;
	clip.frameDuration = 0.1f;
	clip.frameRect = sf::IntRect({0, 0}, {64, 64});
	return {{1, clip}};
}

// --- Basic NPC creation test ---
TEST(NPCCreationTest, CreatesAllExpectedComponents) {
	entt::registry registry;

	sf::Vector2f pos{10.f, 20.f};
	sf::Vector2f targetSize{64.f, 64.f};
	auto clips = makeTestClips();
	float speed = 3.5f;

	entt::entity npc = systems::createNPC(registry, pos, targetSize, clips, speed);

	// Check that the entity exists and has all the required components
	EXPECT_TRUE((registry.all_of<engine::Position, engine::Speed, engine::Velocity,
								 engine::Renderable, engine::Animation>(npc)));

	// Checking Position and Speed
	const auto &position = registry.get<engine::Position>(npc);
	const auto &npcSpeed = registry.get<engine::Speed>(npc);
	EXPECT_NEAR(position.value.x, pos.x, TOLERANCE);
	EXPECT_NEAR(position.value.y, pos.y, TOLERANCE);
	EXPECT_NEAR(npcSpeed.value, speed, TOLERANCE);

	// Check Velocity (must be zero)
	const auto &velocity = registry.get<engine::Velocity>(npc);
	EXPECT_NEAR(velocity.value.x, 0.f, TOLERANCE);
	EXPECT_NEAR(velocity.value.y, 0.f, TOLERANCE);

	// Checking Renderable
	const auto &render = registry.get<engine::Renderable>(npc);
	EXPECT_EQ(render.textureName, "wolf.png");
	EXPECT_EQ(render.textureRect, sf::IntRect({0, 0}, {64, 64}));
	EXPECT_NEAR(render.targetSize.x, 64.f, TOLERANCE);
	EXPECT_NEAR(render.targetSize.y, 64.f, TOLERANCE);

	// Animation Check
	const auto &anim = registry.get<engine::Animation>(npc);
	EXPECT_EQ(anim.state, 1);
	EXPECT_EQ(anim.clips.size(), 1);
	EXPECT_EQ(anim.clips.at(1).texture, "wolf.png");
	EXPECT_EQ(anim.clips.at(1).frameCount, 4);
	EXPECT_NEAR(anim.clips.at(1).frameDuration, 0.1f, TOLERANCE);
}

// --- Check multiple clips ---
TEST(NPCCreationTest, ChoosesFirstClipAsState) {
	entt::registry registry;
	sf::Vector2f pos{5.f, 5.f};
	sf::Vector2f size{32.f, 32.f};

	engine::AnimationClip idle{"idle.png", 4, 0.1f, sf::IntRect({0, 0}, {32, 32})};
	engine::AnimationClip walk{"walk.png", 6, 0.08f, sf::IntRect({0, 0}, {32, 32})};

	std::unordered_map<int, engine::AnimationClip> clips = {{0, idle}, {1, walk}};

	entt::entity npc = systems::createNPC(registry, pos, size, clips, 2.f);
	const auto &anim = registry.get<engine::Animation>(npc);

	// Check that the first clip in the iteration has become a state
	int firstKey = clips.begin()->first;
	EXPECT_EQ(anim.state, firstKey);
	EXPECT_TRUE(anim.clips.find(firstKey) != anim.clips.end());
}

// --- Checking the integrity of Renderable and Animation ---
TEST(NPCCreationTest, RenderableAndAnimationShareTexture) {
	entt::registry registry;
	sf::Vector2f pos{0.f, 0.f};
	sf::Vector2f size{64.f, 64.f};

	auto clips = makeTestClips();
	entt::entity npc = systems::createNPC(registry, pos, size, clips, 1.f);

	const auto &render = registry.get<engine::Renderable>(npc);
	const auto &anim = registry.get<engine::Animation>(npc);

	EXPECT_EQ(render.textureName, anim.clips.begin()->second.texture);
	EXPECT_EQ(render.textureRect, anim.clips.begin()->second.frameRect);
}
