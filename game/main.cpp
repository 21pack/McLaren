#include "../engine/engine.h"
#include "loops/game_loop.h"
#include <memory>

int main() {
	engine::Engine engine;

	auto loop = std::make_unique<GameLoop>();
	engine.setLoop(std::move(loop));

	engine.run();

	return 0;
}
