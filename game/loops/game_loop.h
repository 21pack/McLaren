#pragma once

#include "loop.h"
#include "tile.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <entt/entt.hpp>
#include <vector>

namespace engine {
struct Engine;
struct Camera;
struct RenderFrame;
} // namespace engine

class GameLoop : public engine::ILoop {
  public:
	GameLoop();
	virtual ~GameLoop() = default;

	void init(engine::Engine &engine) override;
	void update(engine::Input &input, float dt) override;
	void collectRenderData(engine::RenderFrame &frame,
						   engine::Camera &camera) override;
	bool isFinished() const override;

  private:
	entt::registry m_registry;
	engine::Engine *m_engine = nullptr;

	void generateTileMap(std::unordered_map<int, engine::TileData> &tileImages);
	void gameAnimationSystem(float dt);

	int width;
	int height;

	sf::VertexArray m_staticMapPoints;
	std::vector<engine::Tile> tiles;
};
