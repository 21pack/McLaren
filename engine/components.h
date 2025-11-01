#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <unordered_map>

namespace engine {

/**
 * @brief Component representing 2D position in world space.
 */
struct Position {
	sf::Vector2f value;
};

/**
 * @brief Component representing movement speed scalar.
 */
struct Speed {
	float value;
};

/**
 * @brief Component representing 2D velocity vector.
 */
struct Velocity {
	sf::Vector2f value;
};

/**
 * @brief Component representing rotation angle in degrees.
 */
struct Rotation {
	float angle = 0.f;
};

/**
 * @brief Enumeration for cardinal directions used in animations.
 */
enum class Direction { Down = 0, Right = 1, Left = 2, Up = 3 };

/**
 * @brief Component defining an animation clip with timing and frame data.
 */
struct AnimationClip {
	std::string texture;		// Texture name
	int frameCount = 1;			// Number of frames per line
	float frameDuration = 0.1f; // Time of one frame
	sf::IntRect frameRect;		// Size of one frame
};

/**
 * @brief Component managing animation state and playback.
 */
struct Animation {
	std::unordered_map<int, AnimationClip> clips;
	int state = 0;						   // Current state
	int frameIdx = 0;					   // Current frame
	float frameTime = 0.f;				   // Accumulated time
	int row = 0;						   // Current line (direction)
	Direction direction = Direction::Down; // Current direction
};

/**
 * @brief Component defining visual representation of an entity.
 */
struct Renderable {
	std::string textureName; // Used to get texture from your Texture manager
	sf::IntRect textureRect; // Base rect for frame 0
	sf::Vector2f targetSize;
	sf::Color color = sf::Color::White;
};

/**
 * @brief Tag component indicating an entity should cast a shadow.
 */
struct CastsShadow {};

/**
 * @brief Tag component for NPC entities that chase the player.
 */
struct ChasingPlayer {};

/**
 * @brief Tag component identifying player-controlled entities.
 */
struct PlayerControlled {};

} // namespace engine
