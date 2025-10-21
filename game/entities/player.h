#pragma once
#include "../../engine/entity.h"
#include "../../engine/input.h"

namespace game {

struct Player : public engine::Entity {
    Player();

    void render(engine::Renderer &renderer,
                const engine::Camera &camera) override;
    
    void update(float dt) override;

    void handleInput(const engine::Input& input);

  private:
    sf::Vector2f m_velocity;
    float m_speed = 5.0f; // 5 tiles per second
};

} // namespace game
