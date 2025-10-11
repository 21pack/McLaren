#pragma once
#include "../my_world.h"
#include "engine/loop.h"
#include "engine/world.h"
#include "entities/player.h"

namespace game {

struct GameLoop : public engine::ILoop {
	GameLoop();

	void update(float dt) override;
	void draw(engine::Renderer &renderer, engine::Camera &camera) override;
	bool isFinished() const override { return m_finished; }

	void exit() override { m_finished = true; }

  private:
	MyWorld world;
	Player player;
};

} // namespace game
