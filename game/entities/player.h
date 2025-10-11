#pragma once
#include "engine/camera.h"
#include "engine/entity.h"
#include "engine/input.h"
#include "engine/renderer.h"

namespace game {

struct Player : public engine::Entity {
	void update(float dt) override;
	void render(engine::Renderer &renderer, const engine::Camera &camera) override;
	void handleInput(const engine::Input &input);

  private:
	float speed = 10.f;
	sf::Vector2f velocity{0.f, 0.f}; // current speed (TODO: needs?)
	float direction = 0.f;
	int currentFrame = 0;
	float frameTimer = 0.f;
};

} // namespace game
