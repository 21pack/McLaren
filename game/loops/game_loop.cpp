#include "game_loop.h"
#include "../../engine/components.h"
#include "../../engine/engine.h"
#include "../../engine/render.h"
#include "../../engine/systems.h"
#include "../../engine/texture.h"
#include "../../engine/tile.h"
#include <memory>

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

	for (int i = 0; i < 5; i++) {
		auto npc = m_registry.create();
		m_registry.emplace<engine::Position>(npc, sf::Vector2f{i * 4.f, i * 4.f});
		m_registry.emplace<engine::Rotation>(npc, 0.f);
		m_registry.emplace<engine::Renderable>(npc, "game/assets/player.png",
											   sf::IntRect({0, 0}, {600, 504}),
											   targetPudgeSize);
	}
}

void GameLoop::buildStaticWorld() {
	const int tileWidth = 64;
	const int tileHeight = 32;
	const int tilesPerRow = 16;

	const int tileGrass = 0;
	const int tileWater = 304;
	const int tileCliffBottom = 33;
	const int tileCliffRight = 36;
	const int tileCliffRightBottomAngle = 80;

	auto getIndex = [&](int x, int y) { return y * width + x; };

	// Fill the entire map with grass
	const engine::Tile defaultGrassTile{tileGrass, false};
	std::fill(tiles.begin(), tiles.end(), defaultGrassTile);

	// Water
	for (int y = 5; y < height - 5; ++y) {
		for (int x = 5; x < 15; ++x) {
			tiles[getIndex(x, y)].id = tileWater;
			tiles[getIndex(x, y)].solid = true;
		}
	}

	// 4. Rocky cliff at the edges of the map
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			if (y == height - 3 && x < width - 3) {
				tiles[getIndex(x, y)].id = tileCliffBottom;
				tiles[getIndex(x, y)].solid = true;
			} else if (y == height - 2 && x < width - 2) {
				tiles[getIndex(x, y)].id = tileCliffBottom + tilesPerRow;
				tiles[getIndex(x, y)].solid = true;
			} else if (y == height - 1 && x < width - 1) {
				tiles[getIndex(x, y)].id = tileCliffBottom + 2 * tilesPerRow;
				tiles[getIndex(x, y)].solid = true;
			}

			if (x == width - 3 && y < height - 3) {
				tiles[getIndex(x, y)].id = tileCliffRight;
				tiles[getIndex(x, y)].solid = true;
			} else if (x == width - 2 && y < height - 2) {
				tiles[getIndex(x, y)].id = tileCliffRight + tilesPerRow;
				tiles[getIndex(x, y)].solid = true;
			} else if (x == width - 1 && y < height - 1) {
				tiles[getIndex(x, y)].id = tileCliffRight + 2 * tilesPerRow;
				tiles[getIndex(x, y)].solid = true;
			}

			if (x == width - 3 && y == height - 3) {
				tiles[getIndex(x, y)].id = tileCliffRightBottomAngle;
				tiles[getIndex(x, y)].solid = true;
			} else if (x == width - 2 && y == height - 2) {
				tiles[getIndex(x, y)].id = tileCliffRightBottomAngle + tilesPerRow;
				tiles[getIndex(x, y)].solid = true;
			} else if (x == width - 1 && y == height - 1) {
				tiles[getIndex(x, y)].id =
					tileCliffRightBottomAngle + 2 * tilesPerRow;
				tiles[getIndex(x, y)].solid = true;
			}
		}
	}

	int texWidth = (width + height) * (tileWidth / 2);
	int texHeight = (width + height) * (tileHeight / 2);

	m_engine->camera.setTileSize((float)tileWidth, (float)tileHeight);

	m_staticMapTexture = std::make_unique<sf::RenderTexture>(
		sf::RenderTexture(sf::Vector2u(texWidth, texHeight)));
	m_staticMapTexture->clear(sf::Color::Transparent);

	auto &textureManager = m_engine->texture;

	sf::Sprite tileSprite(
		textureManager.getTexture("game/assets/grassland_tiles.png"));

	auto getTileRect = [&](int tileId) -> sf::IntRect {
		int tileX = tileId % tilesPerRow;
		int tileY = tileId / tilesPerRow;

		return sf::IntRect({tileX * tileWidth, tileY * tileHeight},
						   {tileWidth, tileHeight});
	};

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			const int tileId = tiles[getIndex(x, y)].id;

			sf::IntRect tileRect = getTileRect(tileId);
			tileSprite.setTextureRect(tileRect);

			sf::Vector2f screenPos =
				m_engine->camera.worldToScreen({(float)x, (float)y});

			screenPos.x += texWidth / 2.0f - tileWidth / 2.0f;
			screenPos.y += tileHeight / 2.0f;

			tileSprite.setPosition(screenPos);

			m_staticMapTexture->draw(tileSprite);
		}
	}

	m_staticMapTexture->display();
	m_staticMapSprite =
		std::make_unique<sf::Sprite>(m_staticMapTexture->getTexture());
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

	engine::RenderFrame::SpriteData mapSprite;
	mapSprite.texture = &m_staticMapTexture->getTexture();
	mapSprite.textureRect =
		sf::IntRect({0, 0}, {(int)m_staticMapTexture->getSize().x,
							 (int)m_staticMapTexture->getSize().y});
	mapSprite.position = {-(m_staticMapTexture->getSize().x / 2.0f),
						  -(m_staticMapTexture->getSize().y / 2.0f)};
	mapSprite.scale = {1.f, 1.f};
	mapSprite.rotation = sf::Angle::Zero;
	mapSprite.color = sf::Color::White;

	frame.sprites.push_back(mapSprite);

	// Collecting entities

	systems::renderSystem(m_registry, frame, camera, m_engine->texture);
}

bool GameLoop::isFinished() const { return m_finished; }
