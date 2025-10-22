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

void Entity::collectRenderData(RenderFrame &frame, const Camera &camera) const {
	if (!m_sprite)
		return;

	RenderFrame::SpriteData spriteData;

	spriteData.texture = &m_sprite->getTexture();
	spriteData.textureRect = m_sprite->getTextureRect();

	spriteData.position = camera.worldToScreen(position);
	spriteData.rotation = m_sprite->getRotation();
	spriteData.scale = m_sprite->getScale();
	spriteData.color = m_sprite->getColor();

	frame.sprites.push_back(spriteData);
}

void Entity::setTexture(sf::Texture &texture, const sf::IntRect &rect) {
	if (!m_sprite) {
		m_sprite = std::make_unique<sf::Sprite>(texture);
	} else {
		m_sprite->setTexture(texture);
	}

	if (rect.size.x != 0 && rect.size.y != 0) {
		m_sprite->setTextureRect(rect);
	}
}

} // namespace engine
