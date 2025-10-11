#include "engine/engine.h"
#include "game/loops/game_loop.h"
#include "game/world_impl.h"

int main() {
	engine::Engine engine;
	// world = ...

	// engine.setLoop(std::make_unique<game::GameLoop>(*world));

	engine.run();

	return 0;
}
