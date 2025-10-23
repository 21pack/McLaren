#pragma once
#include "../../engine/camera.h"
#include "../../engine/loop.h"
#include "../../engine/render_frame.h"
#include "../../engine/tile.h"
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <vector>

namespace engine {
struct Engine;
}

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

	void buildStaticWorld();

	int width;
	int height;

	std::unique_ptr<sf::RenderTexture> m_staticMapTexture;
	std::unique_ptr<sf::Sprite> m_staticMapSprite;
	std::vector<engine::Tile> tiles;
};
