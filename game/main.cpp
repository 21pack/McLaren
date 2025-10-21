#include "../engine/engine.h"
#include "loops/game_loop.h"
#include "game_world.h"
#include <memory>

int main() {
	engine::Engine engine;

	engine.setLoop(std::make_unique<game::GameLoop>());

	engine.run();

	return 0;
}
