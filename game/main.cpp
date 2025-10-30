#include "engine.h"
#include "loops/game_loop.h"
#include <memory>

int main() {

	auto loop = std::make_unique<GameLoop>();
	engine::Engine *e = engine::Engine::withLoop(std::move(loop));
	e->run();
	return 0;
}
