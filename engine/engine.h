#pragma once

namespace engine {

struct Engine {
	Renderer renderer; // responsible for low-level rendering
	Input input;	   // input system
	Camera camera;
	LoopPtr activeLoop; // current game scene (game, menu, etc.)
						// TODO: may not be necessary, but I think its cool

	void setLoop(LoopPtr loop); // switching the active scene
	void run();					// main loop
};

} // namespace engine
