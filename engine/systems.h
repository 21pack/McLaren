#pragma once

#include "components.h"
#include "tile.h"
#include <entt/entt.hpp>

namespace engine {
struct Input;
struct RenderFrame;
struct Camera;
struct ImageManager;
} // namespace engine

namespace systems {

void playerInputSystem(entt::registry &registry, const engine::Input &input);

void movementSystem(entt::registry &registry, std::vector<engine::Tile> tiles,
					int worldWidth, int worldHeight, float dt);

void animationSystem(entt::registry &registry, float dt);

void renderSystem(entt::registry &registry, engine::RenderFrame &frame,
				  const engine::Camera &camera, engine::ImageManager &imageManager);

void npcFollowPlayerSystem(entt::registry &registry, float dt);

void npcWanderSystem(entt::registry &registry, float dt);

entt::entity createNPC(entt::registry &registry, const sf::Vector2f &pos,
					   const sf::Vector2f &targetSize,
					   const std::unordered_map<int, engine::AnimationClip> &clips,
					   float speed);

} // namespace systems
