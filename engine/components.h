#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

namespace engine {

struct Position {
	sf::Vector2f value;
};

struct Velocity {
	sf::Vector2f value;
};

struct Rotation {
	float angle = 0.f; // in degrees
};

struct Animation {
	int frameIdx = 0;
	int frameCount = 1;
	float frameTime = 0.f;
	float frameDuration = 0.1f;
};

struct Renderable {
	std::string textureName; // used to get texture from your Texture manager
	sf::IntRect textureRect; // base rect for frame 0
	sf::Vector2f targetSize;
	sf::Color color = sf::Color::White;
};

// tag component to idendify which entity controlled by user
struct PlayerControlled {};

} // namespace engine
