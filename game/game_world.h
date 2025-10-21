#pragma once
#include "../engine/camera.h"
#include "../engine/entity.h"
#include "../engine/input.h"
#include "../engine/renderer.h"
#include "../engine/world.h"
#include "../engine/tile.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <vector>
#include <memory>
#include <algorithm>

namespace game {

struct GameWorld : public engine::World {
	GameWorld(int w, int h);

	void update(float dt) override;
	void draw(engine::Renderer &renderer, const engine::Camera &camera) override;

	void addEntity(std::unique_ptr<engine::Entity> entity) override;
	void removeEntity(engine::EntityId id) override;

	const std::vector<std::unique_ptr<engine::Entity>> &getEntities() const override;

	engine::Tile *getTileAt(int x, int y) override; 
    engine::Entity *findEntityAtTile(int x, int y) override;

	sf::Vector2i getSize() const { return {width, height}; }

  private:
	int width, height;
	std::vector<engine::Tile> tiles;

	std::vector<std::unique_ptr<engine::Entity>> m_entities;

	sf::VertexArray m_tileVertices; 
	sf::VertexArray m_tileOutlines; 
};

} // namespace game
