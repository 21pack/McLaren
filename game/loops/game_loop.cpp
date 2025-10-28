#include "game_loop.h"
#include "components.h"
#include "engine.h"
#include "image_manager.h"
#include "render.h"
#include "systems.h"
#include "tile.h"
#include <memory>
#include <random>

GameLoop::GameLoop() : width(25), height(25) { tiles.resize(width * height); }

void GameLoop::init(engine::Engine &engine) {
	m_engine = &engine;

	sf::Vector2f worldCenter = {width / 2.0f, height / 2.0f};
	sf::Vector2f screenCenter = m_engine->camera.worldToScreen(worldCenter);
	m_engine->camera.position = screenCenter;

	// Generate tiles once
	auto &imageManager = m_engine->imageManager;

	std::unordered_map<int, engine::TileData> tileImages = {
		{0,
		 {&imageManager.getImage("game/assets/tileset/tile_022.png"), 0}}, // grass
		{1, {&imageManager.getImage("game/assets/tileset/tile_000.png"), 0}}, // sand
		{2,
		 {&imageManager.getImage("game/assets/tileset/tile_104.png"), -2}}, // water
		{3,
		 {&imageManager.getImage("game/assets/tileset/tile_064.png"), 2}}, // rocks
	};

	const int tileWidth = 32.f;
	const int tileHeight = 32.f;
	m_engine->camera.setTileSize(tileWidth, tileHeight / 2);

	generateTileMap(tileImages);
	m_engine->render.generateTileMapVertices(m_staticMapPoints, m_engine->camera,
											 tiles, width, height, tileImages);

	// Creating entities (player, NPC, etc.)

	sf::Vector2f targetWolfSize{64.f, 64.f};
	sf::IntRect frameRect({0, 0}, {64, 64});

	// Wolf
	std::unordered_map<int, engine::AnimationClip> wolfClips = {
		{0, {"game/assets/critters/wolf/wolf-idle.png", 4, 0.15f, frameRect}},
		{1, {"game/assets/critters/wolf/wolf-run.png", 8, 0.08f, frameRect}},
	};

	auto wolf =
		systems::createNPC(m_registry, {0.f, 0.f}, targetWolfSize, wolfClips, 5.f);
	m_registry.emplace<engine::PlayerControlled>(wolf);

	auto wolf1 =
		systems::createNPC(m_registry, {8.f, 8.f}, targetWolfSize, wolfClips, 2.5f);
	m_registry.emplace<engine::ChasingPlayer>(wolf1);

	for (int i = 0; i < 2; i++) {
		systems::createNPC(m_registry, {i + 10.f, i + 10.f}, targetWolfSize,
						   wolfClips, 1.f);
	}
}

void GameLoop::generateTileMap(
	std::unordered_map<int, engine::TileData> &tileImages) {

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
}

void GameLoop::gameAnimationSystem(float dt) {
	auto view =
		m_registry.view<engine::Animation, engine::Velocity, engine::Renderable>();

	for (auto entity : view) {
		auto &anim = view.get<engine::Animation>(entity);
		auto &vel = view.get<engine::Velocity>(entity);
		auto &render = view.get<engine::Renderable>(entity);

		int newState =
			(std::sqrt(vel.value.x * vel.value.x + vel.value.y * vel.value.y) > 0.1f)
				? 1
				: 0;

		if (anim.state != newState) {
			anim.state = newState;
			anim.frameIdx = 0;
			anim.frameTime = 0.f;
		}

		const auto &clip = anim.clips.at(anim.state);
		render.textureName = clip.texture;
		render.textureRect = clip.frameRect;
	}
}

void GameLoop::update(engine::Input &input, float dt) {
	systems::playerInputSystem(m_registry, input);
	systems::npcFollowPlayerSystem(m_registry, dt);
	systems::npcWanderSystem(m_registry, dt);
	systems::movementSystem(m_registry, dt);
	systems::animationSystem(m_registry, dt);
	gameAnimationSystem(dt);

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
