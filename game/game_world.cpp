#include "game_world.h"
#include "../engine/tile.h"
#include <SFML/Graphics/ConvexShape.hpp>

namespace game {

GameWorld::GameWorld(int w, int h) : width(w), height(h) {
    tiles.resize(width * height);

    m_tileVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_tileVertices.resize(width * height * 6); 

    m_tileOutlines.setPrimitiveType(sf::PrimitiveType::Lines);
    m_tileOutlines.resize(width * height * 8);
}

engine::Tile *GameWorld::getTileAt(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return nullptr;
    }
    return &tiles[y * width + x]; 
}

engine::Entity *GameWorld::findEntityAtTile(int x, int y) {
    return nullptr;
}

void GameWorld::update(float dt) {
    for (auto &entity : m_entities) {
        entity->update(dt);
    }
}

void GameWorld::draw(engine::Renderer &renderer, const engine::Camera &camera) {
    sf::Color tileFillColor = sf::Color(20,190,20);
    sf::Color tileOutlineColor = sf::Color(80,50,80);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            sf::Vector2f p0 = {(float)x, (float)y};
            sf::Vector2f p1 = {(float)x + 1, (float)y};
            sf::Vector2f p2 = {(float)x + 1, (float)y + 1};
            sf::Vector2f p3 = {(float)x, (float)y + 1};

            sf::Vector2f s0 = camera.worldToScreen(p0);
            sf::Vector2f s1 = camera.worldToScreen(p1);
            sf::Vector2f s2 = camera.worldToScreen(p2);
            sf::Vector2f s3 = camera.worldToScreen(p3);
            
            size_t tileIndex = y * width + x;

            sf::Vertex* triangles = &m_tileVertices[tileIndex * 6];
            triangles[0].position = s0;
            triangles[1].position = s1;
            triangles[2].position = s3;
            triangles[3].position = s1;
            triangles[4].position = s2;
            triangles[5].position = s3;
            for(int i = 0; i < 6; ++i) triangles[i].color = tileFillColor;

            sf::Vertex* lines = &m_tileOutlines[tileIndex * 8];
            lines[0].position = s0; lines[1].position = s1;
            lines[2].position = s1; lines[3].position = s2;
            lines[4].position = s2; lines[5].position = s3;
            lines[6].position = s3; lines[7].position = s0;
            for(int i = 0; i < 8; ++i) lines[i].color = tileOutlineColor;
        }
    }

    renderer.draw(m_tileVertices);
    renderer.draw(m_tileOutlines);

    for (auto &entity : m_entities) {
        entity->render(renderer, camera);
    }
}

void GameWorld::addEntity(std::unique_ptr<engine::Entity> entity) {
    entity->world = this;
    m_entities.push_back(std::move(entity));
}

void GameWorld::removeEntity(engine::EntityId id) {
    // TODO
}

const std::vector<std::unique_ptr<engine::Entity>> &GameWorld::getEntities() const {
    return m_entities;
}


} // namespace game