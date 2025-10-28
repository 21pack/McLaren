#include "systems.h"
#include "camera.h"
#include "image_manager.h"
#include "input.h"
#include "render_frame.h"
#include <cmath>
#include <random>

namespace systems {

using namespace engine;

void playerInputSystem(entt::registry &registry, const Input &input) {
	auto view = registry.view<Velocity, PlayerControlled, Animation>();

	for (auto entity : view) {
		auto &vel = view.get<Velocity>(entity);
		auto &anim = view.get<Animation>(entity);

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

			if (std::abs(vel.value.x) > std::abs(vel.value.y)) {
				anim.row = (vel.value.x > 0.f) ? 1 : 2; // right=1, left=2
			} else {
				anim.row = (vel.value.y > 0.f) ? 0 : 3; // down=0, up=3
			}
		}
	}
}

void movementSystem(entt::registry &registry, float dt) {
	auto view = registry.view<Position, const Velocity, const Speed>();

	for (auto entity : view) {
		auto &pos = view.get<Position>(entity);
		const auto &vel = view.get<Velocity>(entity);
		const auto &speed = view.get<const Speed>(entity);

		pos.value += vel.value * speed.value * dt;
	}
}

void animationSystem(entt::registry &registry, float dt) {
	auto view = registry.view<Animation>();

	for (auto entity : view) {
		auto &anim = view.get<Animation>(entity);
		const auto it = anim.clips.find(anim.state);
		if (it == anim.clips.end())
			continue;

		const auto &clip = it->second;

		if (clip.frameCount <= 1)
			continue;

		anim.frameTime += dt;
		if (anim.frameTime >= clip.frameDuration) {
			anim.frameTime -= clip.frameDuration;
			anim.frameIdx = (anim.frameIdx + 1) % clip.frameCount;
		}
	}
}

void renderSystem(entt::registry &registry, RenderFrame &frame, const Camera &camera,
				  ImageManager &imageManager) {

	auto view = registry.view<const Position, Renderable, const Velocity>();

	sf::FloatRect boundsCamera = camera.getBounds();
	for (auto entity : view) {
		const auto &pos = view.get<const Position>(entity);
		auto &render = view.get<Renderable>(entity);

		sf::FloatRect boundsEntity(camera.worldToScreen(pos.value),
								   {render.targetSize.x, render.targetSize.y});
		if (!boundsEntity.findIntersection(boundsCamera).has_value()) {
			continue;
		}

		const auto &vel = view.get<const Velocity>(entity);

		// optional components
		auto *anim = registry.try_get<Animation>(entity);
		const auto *rot = registry.try_get<const Rotation>(entity);

		RenderFrame::SpriteData spriteData;
		spriteData.image = &imageManager.getImage(render.textureName);

		// apply animation
		sf::IntRect currentRect = render.textureRect;
		if (anim) {
			currentRect.position.x += currentRect.size.x * anim->frameIdx;
			currentRect.position.y += currentRect.size.y * anim->row;

			const auto &clip = anim->clips.at(anim->state);
			render.textureName = clip.texture;
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

void npcFollowPlayerSystem(entt::registry &registry, float dt) {
	auto playerView = registry.view<const Position, const PlayerControlled>();

	const auto playerEntity = *playerView.begin();
	const auto &playerPos = playerView.get<const Position>(playerEntity);

	auto npcView =
		registry.view<const Position, Velocity, Animation, ChasingPlayer>();

	for (auto entity : npcView) {
		const auto &pos = npcView.get<Position>(entity);
		auto &vel = npcView.get<Velocity>(entity);
		auto &anim = npcView.get<Animation>(entity);

		float length =
			std::sqrt(vel.value.x * vel.value.x + vel.value.y * vel.value.y);
		if (length > 0.01f) {
			sf::Vector2f dir = vel.value / length;

			if (std::abs(dir.x) > std::abs(dir.y)) {
				anim.row = (dir.x > 0.f) ? 1 : 2; // right = 1, left = 2
			} else {
				anim.row = (dir.y > 0.f) ? 0 : 3; // down = 0, up = 3
			}
		}

		sf::Vector2f dir = playerPos.value - pos.value;
		float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

		if (len > 3.f) {
			vel.value = dir / len;
		} else {
			vel.value = {0.f, 0.f};
		}
	}
}

void npcWanderSystem(entt::registry &registry, float dt) {
	auto view = registry.view<Position, Velocity, Animation>(
		entt::exclude<PlayerControlled, ChasingPlayer>);

	for (auto entity : view) {
		auto &vel = view.get<Velocity>(entity);
		auto &anim = view.get<Animation>(entity);

		float length =
			std::sqrt(vel.value.x * vel.value.x + vel.value.y * vel.value.y);
		if (length > 0.01f) {
			sf::Vector2f dir = vel.value / length;

			if (std::abs(dir.x) > std::abs(dir.y)) {
				anim.row = (dir.x > 0.f) ? 1 : 2; // right = 1, left = 2
			} else {
				anim.row = (dir.y > 0.f) ? 0 : 3; // down = 0, up = 3
			}
		}

		vel.value.x += (rand() % 3 - 1) * 0.1f;
		vel.value.y += (rand() % 3 - 1) * 0.1f;

		float len = std::sqrt(vel.value.x * vel.value.x + vel.value.y * vel.value.y);
		if (len > 0.f)
			vel.value /= len;
	}
}

entt::entity createNPC(entt::registry &registry, const sf::Vector2f &pos,
					   const sf::Vector2f &targetSize,
					   const std::unordered_map<int, AnimationClip> &clips,
					   float speed) {

	auto e = registry.create();
	registry.emplace<Position>(e, pos);
	registry.emplace<Speed>(e, speed);
	registry.emplace<Velocity>(e);

	Renderable render;
	render.textureName = clips.begin()->second.texture;
	render.textureRect = clips.begin()->second.frameRect;
	render.targetSize = targetSize;
	registry.emplace<Renderable>(e, std::move(render));

	Animation anim;
	anim.clips = clips;
	anim.state = clips.begin()->first;
	registry.emplace<Animation>(e, std::move(anim));

	return e;
}

} // namespace systems
