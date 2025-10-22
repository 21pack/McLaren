#include "engine.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <thread>

namespace engine {

void Engine::setLoop(LoopPtr loop) {
	activeLoop = std::move(loop);

	if (activeLoop) {
		activeLoop->init(*this);
	}
}

void Engine::run() {
	std::atomic<bool> running = true;

	std::thread updateThread([this, &running]() {
		sf::Clock clock;

		while (render.isOpen()) {
			float dt = clock.restart().asSeconds();
			input.pollEvents(render);

			if (!activeLoop)
				break;

			activeLoop->update(input, dt);

			if (activeLoop->isFinished()) {
				activeLoop.reset();
				running = false;
				break;
			}

			auto newFrame = render.collectFrame(*activeLoop, camera);
			{
				std::lock_guard<std::mutex> lock(renderQueue.mtx);
				renderQueue.backFrame = std::move(newFrame);
				renderQueue.swap();
				renderQueue.updated = true;
			}
		}
	});

	while (render.isOpen() && running) {
		std::shared_ptr<RenderFrame> front = nullptr;

		{
			std::lock_guard<std::mutex> lock(renderQueue.mtx);
			if (renderQueue.updated == true) {
				front = renderQueue.frontFrame;
				renderQueue.updated = false;
			}
		}

		if (front) {
			render.clear();
			render.drawFrame(*front);
			render.present();
		}
	}

	if (render.isOpen())
		render.closeWindow();

	updateThread.join();
}

} // namespace engine
