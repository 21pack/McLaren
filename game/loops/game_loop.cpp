#include "game_loop.h"
#include "../../engine/renderer.h"
#include "../../engine/engine.h"
#include "../../engine/texture.h"

namespace game {

GameLoop::GameLoop() : world(20, 20), player(nullptr) {}

void GameLoop::init(engine::Engine& engine) {
    m_tex = &engine.texture;

    auto playerPtr = std::make_unique<Player>();
    
    sf::Texture& playerTexture = m_tex->getTexture("game/assets/player.png");
    playerPtr->setTexture(playerTexture);

    const float playerHeight = 128.0f; 
    float originalHeight = static_cast<float>(playerTexture.getSize().y);
    float scaleFactor = playerHeight / originalHeight; 
    playerPtr->m_sprite->setScale({scaleFactor, scaleFactor});
    playerPtr->m_sprite->setOrigin({
        static_cast<float>(playerTexture.getSize().x) / 2.f, 
        static_cast<float>(playerTexture.getSize().y)   
    });

    player = playerPtr.get(); 
    player->position = {10.f, 10.f};
    
    world.addEntity(std::move(playerPtr));
}

void GameLoop::update(engine::Input& input, float dt) {
    if (input.isKeyDown(sf::Keyboard::Key::Escape)) {
        exit();
    }

    player->handleInput(input);

    world.update(dt);
}

void GameLoop::draw(engine::Renderer &renderer, engine::Camera &camera) {
    sf::Vector2f windowSize = sf::Vector2f(renderer.getWindow().getSize());
    
    sf::Vector2i worldSize = world.getSize();
    sf::Vector2f worldCenter = {worldSize.x / 2.f, worldSize.y / 2.f};

    engine::Camera tempCamera;
    tempCamera.zoom = camera.zoom; 
    sf::Vector2f projectedWorldCenter = tempCamera.worldToScreen(worldCenter);

    camera.position.x = windowSize.x / 2.f - projectedWorldCenter.x;
    camera.position.y = windowSize.y / 2.f - projectedWorldCenter.y;

    world.draw(renderer, camera);
}

} // namespace game