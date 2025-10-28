#include "systems.h"
#include "camera.h"
#include "components.h"
#include "image_manager.h"
#include "input.h"
#include "render_frame.h"
#include "utils.h"
#include <cmath>
#include <random>

#include <iostream>

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

void movementSystem(entt::registry &registry, std::vector<engine::Tile> tiles,
					int worldWidth, int worldHeight, float dt) {
	auto view = registry.view<Position, const Velocity, const Speed>();

	for (auto entity : view) {
		auto &pos = view.get<Position>(entity);
		auto &vel = view.get<Velocity>(entity);
		const auto &speed = view.get<const Speed>(entity);
		const auto newPos = vel.value * speed.value * dt;
		std::cout << "\nposXY:" << pos.value.x << ":" << pos.value.y << "\n";

		auto getIndex = [&](int x, int y) { return y * worldWidth + x; };
		auto tileX = static_cast<int>(std::floor(pos.value.x + newPos.x));
		auto tileY = static_cast<int>(std::floor(pos.value.y + newPos.y));
		std::cout << "tileXY:" << tileX << ":" << tileY << "\n";

		if (0 <= tileX < worldWidth && 0 <= tileY < worldHeight) {
			auto tile = tiles[getIndex(tileX, tileY)];

			if (!tile.solid) {
				pos.value += newPos;
				std::cout << "posNewXY:" << pos.value.x << ":" << pos.value.y
						  << "\n";
			}
		}
		// pos.value += vel.value * speed.value * dt;
	}
}

void animationSystem(entt::registry &registry, float dt) {
	auto view = registry.view<Animation>();

	for (auto entity : view) {
		auto *anim = registry.try_get<Animation>(entity);
		if (!anim)
			continue;

		auto it = anim->clips.find(anim->state);
		if (it == anim->clips.end())
			continue;

		const auto &clip = it->second;

		if (clip.frameCount <= 1)
			continue;

		anim->frameTime += dt;
		if (anim->frameTime >= clip.frameDuration) {
			anim->frameTime -= clip.frameDuration;
			anim->frameIdx = (anim->frameIdx + 1) % clip.frameCount;
		}
	}
}

void renderSystem(entt::registry &registry, RenderFrame &frame, const Camera &camera,
				  ImageManager &imageManager) {
	sf::FloatRect boundsCamera = camera.getBounds();

	registry.sort<Position>([](const auto &lhs, const auto &rhs) {
		if (lhs.value.y != rhs.value.y) {
			return lhs.value.y < rhs.value.y;
		}
		return lhs.value.x < rhs.value.x;
	});

	auto view = registry.view<const Position, Renderable, const Velocity>();

	const sf::Vector2f shadowVector = {1.f, .0f};
	const sf::Color shadowColor(0, 0, 0, 100);
	const int shadowStep = 1;
	const int pointSize = static_cast<int>(std::ceil(camera.zoom));

	frame.shadowVertices.setPrimitiveType(sf::PrimitiveType::Points);

	for (auto entity : view) {
		const auto &pos = view.get<const Position>(entity);
		auto &render = view.get<Renderable>(entity);
		auto *anim = registry.try_get<Animation>(entity);
		const auto *rot = registry.try_get<const Rotation>(entity);

		sf::IntRect currentFrameRect = render.textureRect;
		const sf::Image *entityImage = &imageManager.getImage(render.textureName);

		if (anim) {
			const auto &clip = anim->clips.at(anim->state);
			entityImage = &imageManager.getImage(clip.texture);

			currentFrameRect.position.x += currentFrameRect.size.x * anim->frameIdx;
			currentFrameRect.position.y += currentFrameRect.size.y * anim->row;
		}

		// calculate content rect in case spritesheet with paddings.
		// TODO: precalculate
		sf::IntRect currentContentRect =
			engine::calculateContentRect(*entityImage, currentFrameRect);

		float frameWidth = static_cast<float>(currentFrameRect.size.x);
		float frameHeight = static_cast<float>(currentFrameRect.size.y);
		float contentWidth = static_cast<float>(currentContentRect.size.x);
		float contentHeight = static_cast<float>(currentContentRect.size.y);

		float uniformScale = camera.zoom;
		if (frameWidth > 0.f && frameHeight > 0.f) {
			float scaleX = render.targetSize.x / frameWidth;
			float scaleY = render.targetSize.y / frameHeight;
			uniformScale = std::min(scaleX, scaleY) * camera.zoom;
		}

		float scaledFrameWidth = frameWidth * uniformScale;
		float scaledFrameHeight = frameHeight * uniformScale;

		const sf::Vector2f anchor = camera.worldToScreen(pos.value);
		const float angle = (rot ? rot->angle * (3.14159f / 180.f) : 0.f);
		const float cosA = std::cos(angle);
		const float sinA = std::sin(angle);

		// generate shadow
		if (registry.all_of<CastsShadow>(entity)) {
			const sf::Image &img = *entityImage;

			int texW = currentContentRect.size.x;
			int texH = currentContentRect.size.y;
			int texLeft = currentContentRect.position.x;
			int texTop = currentContentRect.position.y;

			float contentAnchorX_tex =
				static_cast<float>(texLeft) + contentWidth * 0.5f;
			float contentAnchorY_tex = static_cast<float>(texTop) + contentHeight;

			for (int ty = 0; ty < texH; ty += shadowStep) {
				for (int tx = 0; tx < texW; tx += shadowStep) {

					int u = currentFrameRect.position.x + texLeft + tx;
					int v = currentFrameRect.position.y + texTop + ty;

					if (u < 0 || v < 0 || u >= static_cast<int>(img.getSize().x) ||
						v >= static_cast<int>(img.getSize().y))
						continue;
					if (img.getPixel({(unsigned int)u, (unsigned int)v}).a == 0)
						continue;

					float localX =
						(static_cast<float>(texLeft + tx) - contentAnchorX_tex) *
						uniformScale;
					float localY =
						(static_cast<float>(texTop + ty) - contentAnchorY_tex) *
						uniformScale;

					float rotatedX = localX * cosA - localY * sinA;
					float rotatedY = localX * sinA + localY * cosA;
					float z = -rotatedY;

					float shadowX = (anchor.x + rotatedX) + (z * shadowVector.x);
					float shadowY = (anchor.y + rotatedY) + (z * shadowVector.y);

					for (int dy = 0; dy < pointSize; ++dy) {
						for (int dx = 0; dx < pointSize; ++dx) {
							frame.shadowVertices.append(
								{{shadowX + dx, shadowY + dy}, shadowColor});
						}
					}
				}
			}
		}

		// draw sprite's content at bottom middle
		sf::IntRect absoluteContentRect = currentContentRect;
		absoluteContentRect.position.x += currentFrameRect.position.x;
		absoluteContentRect.position.y += currentFrameRect.position.y;

		float contentWidth_scaled =
			static_cast<float>(currentContentRect.size.x) * uniformScale;
		float contentHeight_scaled =
			static_cast<float>(currentContentRect.size.y) * uniformScale;

		float localX_tl = -contentWidth_scaled * 0.5f;
		float localY_tl = -contentHeight_scaled;

		float rotatedX_tl = localX_tl * cosA - localY_tl * sinA;
		float rotatedY_tl = localX_tl * sinA + localY_tl * cosA;

		sf::Vector2f spriteDrawPos = anchor + sf::Vector2f(rotatedX_tl, rotatedY_tl);

		RenderFrame::SpriteData spriteData;
		spriteData.image = entityImage;
		spriteData.textureRect = absoluteContentRect;
		spriteData.scale = {uniformScale, uniformScale};
		spriteData.position = spriteDrawPos;
		spriteData.rotation = sf::degrees(angle / (3.14159f / 180.f));
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
