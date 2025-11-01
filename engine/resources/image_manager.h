#pragma once

#include <SFML/Graphics/Image.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace engine {

/**
 * @brief Manages loading and storage of SFML Image resources.
 *
 * Provides centralized image loading with caching to avoid duplicate loads.
 * Uses unique_ptr for automatic memory management of image resources.
 */
class ImageManager {
  public:
	/**
	 * @brief Loads and returns a reference to an image from file.
	 * @param filename Path to the image file to load.
	 * @return Reference to the loaded SFML Image object.
	 *
	 * If the image is already loaded, returns the cached version.
	 * Otherwise loads the image from disk and caches it.
	 */
	sf::Image &getImage(const std::string &filename);

  private:
	std::unordered_map<std::string, std::unique_ptr<sf::Image>>
		m_images; ///< Cache of loaded images
};

} // namespace engine
