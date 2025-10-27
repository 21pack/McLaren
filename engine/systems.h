#pragma once
#include <entt/entt.hpp>

namespace engine {
struct Input;
struct RenderFrame;
struct Camera;
struct ImageManager;
} // namespace engine

namespace systems {

void playerInputSystem(entt::registry &registry, const engine::Input &input);

void movementSystem(entt::registry &registry, float dt);

void animationSystem(entt::registry &registry, float dt);

void renderSystem(entt::registry &registry, engine::RenderFrame &frame,
				  const engine::Camera &camera, engine::ImageManager &imageManager);

} // namespace systems
