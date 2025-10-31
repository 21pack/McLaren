#pragma once

#include "camera.h"
#include "image_manager.h"
#include "input.h"
#include "loop.h"
#include "render.h"

namespace engine {

class Engine {
  public:
	void setLoop(LoopPtr loop); // switching the active scene / create engine
	void run();					// main loop
	Engine(Engine const &) = delete;
	void operator=(Engine const &) = delete;
	static Engine *withLoop(LoopPtr loop); // create / get with switched loop
	static Engine *get();				   // return current engine
	Render render;						   // responsible for low-level rendering
	Input input;						   // input system
	Camera camera;
	RenderQueue renderQueue;
	LoopPtr activeLoop; // current game scene (game, menu, etc.)
	ImageManager imageManager;

  private:
	sf::Clock fpsClock;

	void renderLoop();
	void drawLoop();
	Engine() {}
};

} // namespace engine
