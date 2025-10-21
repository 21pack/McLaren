#pragma once

#include <vector>
#include <memory>
#include "entity.h"
#include "tile.h"

namespace engine {

struct Renderer;
struct Camera;
struct EntityId;

// В теории, можно можно абстрагировать мир от тайлов, но тогда будет сложнее
// взаимодействовать с другимим entity: надо будет в цикле проверять для всех entity,
// что они в одном тайле (мб можно как-то это и лучше сделать, но не знаю как)

struct World {
	virtual void update(float dt) = 0; // Updating all entities and world logic
	virtual void draw(Renderer &r,
					  const Camera &c) = 0; // Drawing the world (all entities)
	virtual const std::vector<std::unique_ptr<Entity>> &getEntities() const = 0;
	virtual void addEntity(std::unique_ptr<Entity> entity) = 0;
	virtual void removeEntity(EntityId id) = 0;

	virtual Tile *getTileAt(int x, int y) = 0; // Getting a tile by coordinates
	virtual Entity *findEntityAtTile(int x,
									 int y) = 0; // Search for entity on a tile
};

} // namespace engine
