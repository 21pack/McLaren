#pragma once
#include "../../engine/camera.h"
#include "../../engine/entity.h"
#include "../../engine/input.h"
#include "../../engine/render.h"

namespace game {

struct Player : public engine::Entity {
	Player();

	void update(float dt) override;

	void handleInput(const engine::Input &input);

  private:
	sf::Vector2f m_velocity;
	float m_speed = 5.0f; // 5 tiles per second
};

} // namespace game
