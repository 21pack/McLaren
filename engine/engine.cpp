#include "engine.h"

#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
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

		while (running) {
			float dt = clock.restart().asSeconds();

			if (!activeLoop) {
				running = false;
				break;
			}

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

			sf::sleep(sf::milliseconds(5));
		}
	});

	int frameCount = 0;
	fpsClock.restart();

	while (render.isOpen() && running) {
		if (input.pollEvents(render)) {
			running = false;
			break;
		}

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

			frameCount++;
			float elapsed = fpsClock.getElapsedTime().asSeconds();
			if (elapsed >= 1.f) {
				float fps = static_cast<float>(frameCount) / elapsed;
				std::cout << "FPS: " << static_cast<int>(fps) << "\n";
				frameCount = 0;
				fpsClock.restart();
			}

			render.present();
		} else {
			sf::sleep(sf::milliseconds(1));
		}
	}

	if (render.isOpen()) {
		render.closeWindow();
	}

	updateThread.join();
}

} // namespace engine
