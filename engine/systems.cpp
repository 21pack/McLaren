#include "systems.h"
#include "camera.h"
#include "components.h"
#include "image_manager.h"
#include "input.h"
#include "render_frame.h"
#include <cmath>

namespace systems {

using namespace engine;

void playerInputSystem(entt::registry &registry, const engine::Input &input) {
	auto view = registry.view<Velocity, const PlayerControlled>();

	for (auto entity : view) {
		auto &vel = view.get<Velocity>(entity);
		vel.value = {0.f, 0.f};

		// movement in world coordinates
		if (input.isKeyDown(sf::Keyboard::Key::W)) {
			vel.value.y -= 1.f;
		}
		if (input.isKeyDown(sf::Keyboard::Key::S)) {
			vel.value.y += 1.f;
		}
		if (input.isKeyDown(sf::Keyboard::Key::A)) {
			vel.value.x -= 1.f;
		}
		if (input.isKeyDown(sf::Keyboard::Key::D)) {
			vel.value.x += 1.f;
		}

		float length =
			std::sqrt(vel.value.x * vel.value.x + vel.value.y * vel.value.y);
		if (length > 0.f) {
			vel.value /= length;
		}
	}
}

void movementSystem(entt::registry &registry, float dt) {
	const float speed = 5.f; // 5 tiles per second

	auto view = registry.view<Position, const Velocity>();
	for (auto entity : view) {
		auto &pos = view.get<Position>(entity);
		const auto &vel = view.get<Velocity>(entity);
		pos.value += vel.value * speed * dt;
	}
}

void animationSystem(entt::registry &registry, float dt) {
	auto view = registry.view<Animation>();
	for (auto entity : view) {
		auto &anim = view.get<Animation>(entity);

		if (anim.frameCount <= 1)
			continue;

		anim.frameTime += dt;
		if (anim.frameTime >= anim.frameDuration) {
			anim.frameTime -= anim.frameDuration;
			anim.frameIdx = (anim.frameIdx + 1) % anim.frameCount;
		}
	}
}

void renderSystem(entt::registry &registry, engine::RenderFrame &frame,
				  const engine::Camera &camera, engine::ImageManager &imageManager) {

	auto view = registry.view<const Position, const Renderable>();

	for (auto entity : view) {
		const auto &pos = view.get<const Position>(entity);
		const auto &render = view.get<const Renderable>(entity);

		// optional components
		const auto *anim = registry.try_get<const Animation>(entity);
		const auto *rot = registry.try_get<const Rotation>(entity);

		RenderFrame::SpriteData spriteData;
		spriteData.image = &imageManager.getImage(render.textureName);

		// apply animation
		sf::IntRect currentRect = render.textureRect;
		if (anim && anim->frameCount > 0) {
			currentRect.position.x += currentRect.size.x * anim->frameIdx;
		}
		spriteData.textureRect = currentRect;

		float sourceWidth = static_cast<float>(render.textureRect.size.x);
		float sourceHeight = static_cast<float>(render.textureRect.size.y);

		if (sourceWidth > 0.f && sourceHeight > 0.f) {
			float scaleX = render.targetSize.x / sourceWidth;
			float scaleY = render.targetSize.y / sourceHeight;

			float uniformScale = std::min(scaleX, scaleY);

			uniformScale *= camera.zoom;

			spriteData.scale = {uniformScale, uniformScale};
		} else {
			spriteData.scale = {camera.zoom, camera.zoom};
		}

		// apply isometric position & rotation
		spriteData.position = camera.worldToScreen(pos.value);
		if (rot) {
			spriteData.rotation = sf::degrees(rot->angle);
		} else {
			spriteData.rotation = sf::Angle::Zero;
		}

		spriteData.color = render.color;
		frame.sprites.push_back(spriteData);
	}
}

} // namespace systems
