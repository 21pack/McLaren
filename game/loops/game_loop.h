#pragma once
#include "../../engine/camera.h"
#include "../../engine/loop.h"
#include "../../engine/render_frame.h"
#include "../../engine/tile.h"
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

	int width;
	int height;
	std::vector<engine::Tile> tiles;
};
