#pragma once

#include "camera.h"
#include "engine.h"
#include "image_manager.h"
#include "input.h"
#include "loop.h"
#include "render.h"

namespace engine {

struct Engine {
	Render render; // responsible for low-level rendering
	Input input;   // input system
	Camera camera;
	RenderQueue renderQueue;
	LoopPtr activeLoop; // current game scene (game, menu, etc.)
	ImageManager imageManager;

	void setLoop(LoopPtr loop); // switching the active scene
	void run();					// main loop

  private:
	sf::Clock fpsClock;

	void renderLoop();
	void drawLoop();
};

} // namespace engine
