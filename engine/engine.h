#pragma once

#include "camera.h"
#include "engine.h"
#include "input.h"
#include "loop.h"
#include "render.h"
#include "texture.h"

namespace engine {

struct Engine {
	Render render; // responsible for low-level rendering
	Input input;   // input system
	Camera camera;
	RenderQueue renderQueue;
	LoopPtr activeLoop; // current game scene (game, menu, etc.)
	Texture texture;

	void setLoop(LoopPtr loop); // switching the active scene
	void run();					// main loop

  private:
	void renderLoop();
	void drawLoop();
};

} // namespace engine
