#pragma once

#include "core/render_frame.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <mutex>
#include <unordered_map>

namespace engine {

class Camera;
struct Tile;
struct TileData;
struct RenderFrame;
struct ILoop;

/**
 * @brief Main rendering system handling window management and frame rendering.
 *
 * Manages the SFML render window and provides methods for frame collection,
 * drawing, and tile map generation. Implements double buffering for render frames.
 */
class Render {
  public:
	sf::RenderWindow window; ///< SFML render window for display

	/**
	 * @brief Constructs a Render system with specified window parameters.
	 * @param width Window width in pixels.
	 * @param height Window height in pixels.
	 * @param title Window title string.
	 */
	Render(unsigned width = 1000u, unsigned height = 600u,
		   const char *title = "Game")
		: window(sf::VideoMode({width, height}), title) {}

	bool isOpen() const { return window.isOpen(); } ///< Checks if window is open
	void clear(const sf::Color &color = sf::Color::Black) {
		window.clear(color);
	} ///< Clears window with specified color
	void present() { window.display(); } ///< Displays rendered content

	/**
	 * @brief Collects render data from a game loop into a frame.
	 * @param loop Reference to the active game loop.
	 * @param camera Reference to the camera for view setup.
	 * @return Shared pointer to the collected render frame.
	 */
	std::shared_ptr<RenderFrame> collectFrame(ILoop &loop, Camera &camera);

	/**
	 * @brief Draws a complete render frame to the window.
	 * @param frame Reference to the render frame to draw.
	 */
	void drawFrame(const RenderFrame &frame);

	/**
	 * @brief Generates vertex data for tile-based rendering.
	 * @param vertices Vertex array to populate with tile data.
	 * @param camera Reference to the camera for culling calculations.
	 * @param tiles Vector of tiles in the world.
	 * @param worldWidth Width of the world in tiles.
	 * @param worldHeight Height of the world in tiles.
	 * @param tileImages Map of tile ID to tile visual data.
	 */
	void
	generateTileMapVertices(std::vector<sf::VertexArray> &tileMeshes, Camera &camera,
							const std::vector<Tile> &tiles, int worldWidth,
							int worldHeight,
							std::unordered_map<int, engine::TileData> &tileImages);

	sf::RenderWindow &getWindow() { return window; } ///< Gets the render window
	void closeWindow() { window.close(); }			 ///< Closes the render window

	/**
	 * @brief Collects visible tiles into a vertex array based on camera position.
	 */
	void collectVisibleTiles(const std::vector<sf::VertexArray> &tileMeshes,
							 Camera &camera, const sf::Vector2i &worldSize,
							 sf::VertexArray &outputVertices);

  private:
	/**
	 * @brief Draws an individual sprite to the window.
	 * @param window Reference to the render window.
	 * @param sprite Sprite data to draw.
	 * @param step Rendering step for ordering.
	 */
	void drawSprite(sf::RenderWindow &window, const RenderFrame::SpriteData &sprite,
					int step);

	struct VisibleTilesCache {
		sf::FloatRect cameraBounds;
		sf::VertexArray vertices;
		bool valid = false;
	};

	VisibleTilesCache m_tileCache;
	sf::Vector2i m_lastWorldSize;
};

/**
 * @brief Double-buffered queue for render frame management.
 *
 * Provides thread-safe swapping between front (drawing) and back (rendering) frames
 * to prevent rendering artifacts during frame updates.
 */
class RenderQueue {
  public:
	std::shared_ptr<RenderFrame>
		frontFrame; ///< Frame currently being drawn to screen
	std::shared_ptr<RenderFrame> backFrame; ///< Frame being prepared for next render

	bool updated = false; ///< Flag indicating if new frame data is available
	std::mutex mtx;		  ///< Mutex for thread-safe frame swapping

	/**
	 * @brief Constructs a RenderQueue with initialized frame buffers.
	 */
	RenderQueue() {
		frontFrame = std::make_shared<RenderFrame>();
		backFrame = std::make_shared<RenderFrame>();
	}

	/**
	 * @brief Swaps the front and back buffers.
	 *
	 * Makes the newly prepared back frame available for drawing.
	 */
	void swap() { std::swap(frontFrame, backFrame); }
};

} // namespace engine
