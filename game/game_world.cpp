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

engine::Entity *GameWorld::findEntityAtTile(int x, int y) { return nullptr; }

void GameWorld::update(float dt) {
	for (auto &entity : m_entities) {
		entity->update(dt);
	}
}

void GameWorld::collectRenderData(engine::RenderFrame &frame,
								  const engine::Camera &camera) const {
	sf::Color tileFillColor = sf::Color(20, 190, 20);
	sf::Color tileOutlineColor = sf::Color(80, 50, 80);

	frame.tileVertices.clear();
	frame.tileOutlines.clear();

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

			frame.tileVertices.push_back({s0, tileFillColor});
			frame.tileVertices.push_back({s1, tileFillColor});
			frame.tileVertices.push_back({s3, tileFillColor});
			frame.tileVertices.push_back({s1, tileFillColor});
			frame.tileVertices.push_back({s2, tileFillColor});
			frame.tileVertices.push_back({s3, tileFillColor});

			frame.tileOutlines.push_back({s0, tileOutlineColor});
			frame.tileOutlines.push_back({s1, tileOutlineColor});
			frame.tileOutlines.push_back({s1, tileOutlineColor});
			frame.tileOutlines.push_back({s2, tileOutlineColor});
			frame.tileOutlines.push_back({s2, tileOutlineColor});
			frame.tileOutlines.push_back({s3, tileOutlineColor});
			frame.tileOutlines.push_back({s3, tileOutlineColor});
			frame.tileOutlines.push_back({s0, tileOutlineColor});
		}
	}

	for (auto &entity : m_entities) {
		entity->collectRenderData(frame, camera);
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
