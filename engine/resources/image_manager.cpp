#include "image_manager.h"

#include <iostream>

namespace engine {

sf::Image &ImageManager::getImage(const std::string &filename) {
	auto it = m_images.find(filename);
	if (it != m_images.end()) {
		return *it->second;
	}

	auto image = std::make_unique<sf::Image>();
	if (!image->loadFromFile(filename)) {
		std::cerr << "Error: Could not load texture from file: " << filename
				  << std::endl;
	}

	auto *imgPtr = image.get();
	m_images[filename] = std::move(image);
	return *imgPtr;
}

} // namespace engine
