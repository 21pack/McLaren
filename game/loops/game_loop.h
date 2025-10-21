#pragma once
#include "../game_world.h" 
#include "../../engine/loop.h"
#include "../entities/player.h" 

namespace engine { 
    struct Engine; 
    struct Texture; 
}

namespace game {

struct GameLoop : public engine::ILoop {
    GameLoop();

    void init(engine::Engine& engine) override;
    void update(engine::Input& input, float dt) override; 
    void draw(engine::Renderer &renderer, engine::Camera &camera) override;
    bool isFinished() const override { return m_finished; }

    void exit() override { m_finished = true; }

  private:
    GameWorld world;
    Player* player;
    engine::Texture* m_tex = nullptr;
};

} // namespace game
