#pragma once

#include "render_frame.h"
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

class Render {
  public:
	sf::RenderWindow window;

	Render(unsigned width = 1000u, unsigned height = 600u,
		   const char *title = "Game")
		: window(sf::VideoMode({width, height}), title) {}

	bool isOpen() const { return window.isOpen(); }
	void clear(const sf::Color &color = sf::Color::Black) { window.clear(color); }
	void present() { window.display(); }

	std::shared_ptr<RenderFrame> collectFrame(ILoop &loop, Camera &camera);
	void drawFrame(const RenderFrame &frame);
	void
	generateTileMapVertices(sf::VertexArray &vertices, Camera &camera,
							const std::vector<Tile> &tiles, int worldWidth,
							int worldHeight,
							std::unordered_map<int, engine::TileData> &tileImages);

	sf::RenderWindow &getWindow() { return window; }
	void closeWindow() { window.close(); }

  private:
	void drawSprite(sf::RenderWindow &window, const RenderFrame::SpriteData &sprite,
					int step);
};

class RenderQueue {
  public:
	std::shared_ptr<RenderFrame> frontFrame; // for draw
	std::shared_ptr<RenderFrame> backFrame;	 // for render

	bool updated = false;
	std::mutex mtx;

	RenderQueue() {
		frontFrame = std::make_shared<RenderFrame>();
		backFrame = std::make_shared<RenderFrame>();
	}

	void swap() { std::swap(frontFrame, backFrame); }
};

} // namespace engine
