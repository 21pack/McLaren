#include "texture.h"
#include <iostream> 

namespace engine {

sf::Texture& Texture::getTexture(const std::string& filename) {
    auto it = m_textures.find(filename);
    if (it != m_textures.end()) {
        return *it->second;
    }

    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(filename)) {
        std::cerr << "Error: Could not load texture from file: " << filename << std::endl;
    }

    auto* texPtr = texture.get();
    m_textures[filename] = std::move(texture);
    return *texPtr;
}

} // namespace engine
