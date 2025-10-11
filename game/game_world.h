#pragma once
#include "engine/camera.h"
#include "engine/entity.h"
#include "engine/input.h"
#include "engine/renderer.h"
#include "engine/world.h"
#include <vector>

namespace game {

struct Tile {
	int id = 0;
	bool solid = true;
};

struct GameWorld : public engine::World {
	void update(float dt) override;
	void draw(engine::Renderer &renderer, const engine::Camera &camera) override;

	void addEntity(engine::Entity *entity);
	void removeEntity(engine::EntityId id) override;

	const std::vector<engine::Entity *> &getEntities() const override;

	Tile *getTileAt(int x, int y) override;
	engine::Entity *findEntityAtTile(int x, int y) override;

  private:
	int width, height;
	std::vector<Tile> tiles;
	std::vector<engine::Entity *> entities;

	std::vector<std::vector<std::vector<engine::Entity *>>>
		tileEntities; // entities on each tile
};

} // namespace game
