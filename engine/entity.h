#pragma once
#include "camera.h"
#include "entity.h"
#include "render_frame.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>

namespace engine {

struct World;
struct Render;
struct Camera;
struct RenderFrame;

struct EntityId {
	uint32_t index;
	uint32_t generation;
};

struct Entity {
	EntityId id;				// Unique identifier
	World *world = nullptr;		// Reference to the world the object is in
	sf::Vector2f position;		// Position of the object in the world
	float direction = 0.f;		// Direction
	int frameIdx = 0;			// Current animation frame index
	int frameCount = 1;			// Total number of frames in the animation
	float frameTime = 0.f;		// Timer for switching frames
	float frameDuration = 0.1f; // Time between frames

	std::unique_ptr<sf::Sprite> m_sprite;

	virtual void update(float dt); // Object logic (movement, animation)
	virtual void collectRenderData(RenderFrame &frame, const Camera &camera) const;
	virtual void setTexture(sf::Texture &texture,
							const sf::IntRect &rect = sf::IntRect());
};

} // namespace engine
