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

struct RenderFrame {
	sf::View cameraView;
	sf::Color clearColor = sf::Color::Black;

	struct SpriteData {
		const sf::Image *image = nullptr;	  // pointer to the image
		sf::IntRect textureRect;			  // texture area
		sf::Vector2f position;				  // position in world coordinates
		sf::Angle rotation = sf::Angle::Zero; // turn
		sf::Vector2f scale = {1.f, 1.f};	  // scale
		sf::Color color = sf::Color::White;	  // sprite color
	};

	std::vector<SpriteData> sprites;
	sf::VertexArray tileVertices;
};

} // namespace engine
