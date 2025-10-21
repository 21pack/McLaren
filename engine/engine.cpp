#include "engine.h"
#include "input.h"
#include "loop.h"
#include "renderer.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

namespace engine {

void Engine::setLoop(LoopPtr loop) {
    activeLoop = std::move(loop);

    if (activeLoop) {
        activeLoop->init(*this);
    }
}

void Engine::run() {
    sf::Clock clock;

    while (renderer.isOpen()) {
        float dt = clock.restart().asSeconds();

        input.pollEvents(renderer);

        if (activeLoop) {
            activeLoop->update(input, dt);

            if (activeLoop->isFinished()) {
                activeLoop.reset();   
                renderer.closeWindow();
            }
        }

        renderer.clear();
        if (activeLoop) {
            activeLoop->draw(renderer, camera);
        }
        renderer.present();
    }
}

} // namespace engine