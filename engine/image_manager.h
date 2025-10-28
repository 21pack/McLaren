#pragma once

#include <SFML/Graphics/Image.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace engine {

class ImageManager {
  public:
	sf::Image &getImage(const std::string &filename);

  private:
	std::unordered_map<std::string, std::unique_ptr<sf::Image>> m_images;
};

} // namespace engine
