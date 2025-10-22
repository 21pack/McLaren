// render_frame.h
#pragma once
#include <SFML/Graphics.hpp>
#include <atomic>
#include <memory>
#include <vector>

namespace engine {

struct RenderFrame {
	sf::View cameraView;
	sf::Color clearColor = sf::Color::Black;

	struct SpriteData {
		const sf::Texture *texture = nullptr; // pointer to the texture
		sf::IntRect textureRect;			  // texture area
		sf::Vector2f position;				  // position in world coordinates
		sf::Angle rotation = sf::Angle::Zero; // turn
		sf::Vector2f scale = {1.f, 1.f};	  // scale
		sf::Color color = sf::Color::White;	  // sprite color
	};

	std::vector<SpriteData> sprites;

	std::vector<sf::Vertex> tileVertices;
	std::vector<sf::Vertex> tileOutlines;
};

} // namespace engine
