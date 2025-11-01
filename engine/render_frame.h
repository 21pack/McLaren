#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <atomic>
#include <vector>

namespace engine {

/**
 * @brief Container for all render data collected during a single frame.
 *
 * Holds all the visual information needed to render a complete frame,
 * including sprites, vertices, and camera settings.
 */
struct RenderFrame {
	sf::View cameraView;					 ///< Camera view settings for this frame
	sf::Color clearColor = sf::Color::Black; ///< Background color for the frame

	/**
	 * @brief Data structure for individual sprite rendering.
	 */
	struct SpriteData {
		const sf::Image *image = nullptr; ///< Pointer to the source image texture
		sf::IntRect textureRect; ///< Texture coordinates for sprite sampling
		sf::Vector2f position;	 ///< World position of the sprite
		sf::Angle rotation = sf::Angle::Zero; ///< Rotation angle of the sprite
		sf::Vector2f scale = {1.f, 1.f};	  ///< Scale factors for the sprite
		sf::Color color = sf::Color::White;	  ///< Color tint applied to the sprite
	};

	sf::VertexArray shadowVertices;	 ///< Vertex data for shadow rendering
	std::vector<SpriteData> sprites; ///< Collection of sprites to render this frame
	sf::VertexArray tileVertices;	 ///< Vertex data for tile-based rendering
};

} // namespace engine
