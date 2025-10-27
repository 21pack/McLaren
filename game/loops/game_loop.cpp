#include "game_loop.h"
#include "../../engine/components.h"
#include "../../engine/engine.h"
#include "../../engine/image_manager.h"
#include "../../engine/render.h"
#include "../../engine/systems.h"
#include "../../engine/tile.h"
#include <memory>
#include <random>

GameLoop::GameLoop() : width(50), height(50) { tiles.resize(width * height); }

void GameLoop::init(engine::Engine &engine) {
	m_engine = &engine;

	sf::Vector2f worldCenter = {width / 2.0f, height / 2.0f};
	sf::Vector2f screenCenter = m_engine->camera.worldToScreen(worldCenter);
	m_engine->camera.position = screenCenter;

	// Generate tiles once
	buildStaticWorld();

	// Creating entities (player, NPC, etc.)

	sf::Vector2f targetPudgeSize{128.f, 128.f};

	auto player = m_registry.create();
	m_registry.emplace<engine::PlayerControlled>(player);
	m_registry.emplace<engine::Position>(player, sf::Vector2f{0.f, 0.f});
	m_registry.emplace<engine::Velocity>(player);
	m_registry.emplace<engine::Renderable>(player, "game/assets/player.png",
										   sf::IntRect({0, 0}, {600, 504}),
										   targetPudgeSize);

	for (int i = 0; i < 3; i++) {
		auto npc = m_registry.create();
		m_registry.emplace<engine::Position>(npc, sf::Vector2f{i * 4.f, i * 4.f});
		m_registry.emplace<engine::Rotation>(npc, 0.f);
		m_registry.emplace<engine::Renderable>(npc, "game/assets/player.png",
											   sf::IntRect({0, 0}, {600, 504}),
											   targetPudgeSize);
	}
}

void GameLoop::buildStaticWorld() {
	const int tileWidth = 32;
	const int tileHeight = 32;

	auto &imageManager = m_engine->imageManager;

	struct TileData {
		sf::Image *image;
		int height;
	};

	std::unordered_map<int, TileData> tileImages = {
		{0,
		 {&imageManager.getImage("game/assets/tileset/tile_022.png"), 0}}, // grass
		{1, {&imageManager.getImage("game/assets/tileset/tile_000.png"), 0}}, // sand
		{2,
		 {&imageManager.getImage("game/assets/tileset/tile_104.png"), -2}}, // water
		{3,
		 {&imageManager.getImage("game/assets/tileset/tile_064.png"), 2}}, // rocks
	};

	auto getIndex = [&](int x, int y) { return y * width + x; };

	// Fill the entire map with grass
	const engine::Tile defaultGrassTile{{0}, false};
	std::fill(tiles.begin(), tiles.end(), defaultGrassTile);

	std::mt19937 rng(52);
	std::uniform_int_distribution<int> chance(0, 2);

	auto makeJaggedRegion = [&](int minX, int maxX, int minY, int maxY, int tileId) {
		for (int y = minY; y < maxY; ++y) {
			for (int x = minX; x < maxX; ++x) {
				if (x >= 0 && x < width && y >= 0 && y < height) {
					tiles[getIndex(x, y)] = {{tileId}, true};
				}
			}
		}

		const int margin = 2;
		for (int y = minY - margin; y <= maxY + margin; ++y) {
			for (int x = minX - margin; x <= maxX + margin; ++x) {
				if (x < 0 || x >= width || y < 0 || y >= height)
					continue;

				if (x >= minX && x < maxX && y >= minY && y < maxY)
					continue;

				bool adjacent = false;
				for (int dy = -1; dy <= 1; ++dy) {
					for (int dx = -1; dx <= 1; ++dx) {
						int nx = x + dx, ny = y + dy;
						if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
							if (tiles[getIndex(nx, ny)].layerIds[0] == tileId) {
								adjacent = true;
								break;
							}
						}
					}
					if (adjacent)
						break;
				}

				if (adjacent && chance(rng) == 0) {
					tiles[getIndex(x, y)] = {{tileId}, true};
				}
			}
		}
	};

	// Sand
	makeJaggedRegion(45, width, 0, height, 1);

	// Water
	makeJaggedRegion(7, 15, 9, height - 9, 2);

	// Rocky cliff at the edges of the map
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			if (y == height - 1 && x < width) {
				tiles[getIndex(x, y)] = {{0, 3}, true};
			}

			if (x == width - 1 && y < height) {
				tiles[getIndex(x, y)] = {{0, 3}, true};
			}

			if (x == width - 1 && y == height) {
				tiles[getIndex(x, y)] = {{0, 3}, true};
			}
		}
	}

	m_engine->camera.setTileSize((float)tileWidth, (float)tileHeight / 2);
	m_staticMapPoints.setPrimitiveType(sf::PrimitiveType::Points);
	const int step = 1; // If want to make draw faster, you can increase it

	m_staticMapPoints.clear();

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			const auto &tile = tiles[getIndex(x, y)];
			sf::Vector2f isoVec =
				m_engine->camera.worldToScreen({(float)x, (float)y});

			for (int layerId : tile.layerIds) {
				if (tileImages.find(layerId) == tileImages.end()) {
					continue;
				}

				const TileData &tileData = tileImages[layerId];
				sf::Image &tileImage = *tileData.image;
				int layerHeight = tileData.height;

				for (int ty = 0; ty < tileHeight; ty += step) {
					for (int tx = 0; tx < tileWidth; tx += step) {
						if (tx < 0 || ty < 0 || tx >= tileWidth || ty >= tileHeight)
							continue;

						sf::Color color =
							tileImage.getPixel({(unsigned int)tx, (unsigned int)ty});
						if (color.a == 0)
							continue;

						float pixelX = isoVec.x + tx;
						float pixelY = isoVec.y + ty - layerHeight;

						m_staticMapPoints.append({{pixelX, pixelY}, color});
					}
				}
			}
		}
	}
}

void GameLoop::update(engine::Input &input, float dt) {
	systems::playerInputSystem(m_registry, input);
	systems::movementSystem(m_registry, dt);
	systems::animationSystem(m_registry, dt);

	// camera follow
	auto playerView =
		m_registry.view<const engine::Position, const engine::PlayerControlled>();
	for (auto entity : playerView) {
		const auto &pos = playerView.get<const engine::Position>(entity);
		m_engine->camera.position = m_engine->camera.worldToScreen(pos.value);
	}
}

void GameLoop::collectRenderData(engine::RenderFrame &frame,
								 engine::Camera &camera) {
	// Collecting static map texture
	frame.tileVertices = m_staticMapPoints;

	// Collecting entities
	systems::renderSystem(m_registry, frame, camera, m_engine->imageManager);
}

bool GameLoop::isFinished() const { return m_finished; }
