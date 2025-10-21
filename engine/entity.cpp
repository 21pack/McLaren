#include "entity.h"
#include <iostream>

namespace engine {

void Entity::update(float dt) {
    frameTime += dt;
    if (frameTime >= frameDuration) {
        frameTime -= frameDuration;
        frameIdx = (frameIdx + 1) % frameCount;
    }
}

} // namespace engine