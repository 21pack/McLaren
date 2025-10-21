#include "player.h"
#include "../../engine/renderer.h"
#include "../../engine/camera.h"
// #include <SFML/Graphics/CircleShape.hpp>
#include <cmath>
#include <iostream>

namespace game {

Player::Player() {
    frameCount = 1; 
    m_velocity = {0.f, 0.f};
}

void Player::render(engine::Renderer &renderer, const engine::Camera &camera) {
    if (!m_sprite) {
        return;
    }

    sf::Vector2f screenPos = camera.worldToScreen(position);
    m_sprite->setPosition(screenPos);

    renderer.draw(*m_sprite);
}

void Player::handleInput(const engine::Input& input) {

    m_velocity = {0.f, 0.f};

    if (input.isKeyDown(sf::Keyboard::Key::W)) {
        m_velocity.y -= 1.f;
    }
    if (input.isKeyDown(sf::Keyboard::Key::S)) {
        m_velocity.y += 1.f;
    }
    if (input.isKeyDown(sf::Keyboard::Key::A)) {
        m_velocity.x -= 1.f;
    }
    if (input.isKeyDown(sf::Keyboard::Key::D)) {
        m_velocity.x += 1.f;
    }
    
    float length = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);

    if (length != 0.f) {
        m_velocity /= length;
    } 
    
    m_velocity *= m_speed;

}

void Player::update(float dt) {
    engine::Entity::update(dt); 

    position += m_velocity * dt;
}

} // namespace game