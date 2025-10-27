#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <unordered_map>

namespace engine {

struct Position {
	sf::Vector2f value;
};

struct Speed {
	float value;
};

struct Velocity {
	sf::Vector2f value;
};

struct Rotation {
	float angle = 0.f; // in degrees
};

enum class Direction { Down = 0, Right = 1, Left = 2, Up = 3 };

enum class AnimationState { Idle, Run };

struct AnimationClip {
	std::string texture;		// Texture name
	int frameCount = 1;			// Number of frames per line
	float frameDuration = 0.1f; // Time of one frame
	sf::IntRect frameRect;		// Size of one frame
};

struct Animation {
	std::unordered_map<AnimationState, AnimationClip> clips;
	AnimationState state = AnimationState::Idle; // current state
	int frameIdx = 0;							 // current frame
	float frameTime = 0.f;						 // accumulated time
	int row = 0;								 // current line (direction)
	Direction direction = Direction::Down;		 // current direction
};

struct Renderable {
	std::string textureName; // used to get texture from your Texture manager
	sf::IntRect textureRect; // base rect for frame 0
	sf::Vector2f targetSize;
	sf::Color color = sf::Color::White;
};

// tag for an NPC that is chasing the player
struct ChasingPlayer {};

// tag component to idendify which entity controlled by user
struct PlayerControlled {};

} // namespace engine
