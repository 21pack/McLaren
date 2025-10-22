#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <map>
#include <memory>
#include <string>

namespace engine {

class Texture {
  public:
	sf::Texture &getTexture(const std::string &filename);

  private:
	std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
};

} // namespace engine
