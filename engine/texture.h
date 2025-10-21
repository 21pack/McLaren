#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <map>
#include <memory>

namespace engine {

class Texture {
public:
    sf::Texture& getTexture(const std::string& filename);

private:
    std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
};

} // namespace engine
