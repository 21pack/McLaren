#pragma once
#include "../../engine/camera.h"
#include "../../engine/loop.h"
#include "../../engine/render_frame.h"
#include "../entities/player.h"
#include "../game_world.h"

namespace engine {
struct Engine;
struct Texture;
} // namespace engine

namespace game {

struct GameLoop : public engine::ILoop {
	GameLoop();

	void init(engine::Engine &engine) override;

	void update(engine::Input &input, float dt) override;
	void collectRenderData(engine::RenderFrame &frame,
						   engine::Camera &camera) override;

	bool isFinished() const override { return m_finished; }
	void exit() override { m_finished = true; }

  private:
	GameWorld world;
	Player *player;
	engine::Texture *m_tex = nullptr;
};

} // namespace game
