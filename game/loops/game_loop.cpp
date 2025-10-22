#include "game_loop.h"
#include "../../engine/components.h"
#include "../../engine/engine.h"
#include "../../engine/render.h"
#include "../../engine/systems.h"
#include "../../engine/texture.h"
#include "../../engine/tile.h"
#include <memory>

GameLoop::GameLoop() : width(20), height(20) { tiles.resize(width * height); }

void GameLoop::init(engine::Engine &engine) {
	m_engine = &engine;

	sf::Vector2f worldCenter = {width / 2.0f, height / 2.0f};

	sf::Vector2f screenCenter = m_engine->camera.worldToScreen(worldCenter);

	m_engine->camera.position = screenCenter;

	sf::Vector2f targetPudgeSize{128.f, 128.f};

	auto player = m_registry.create();
	m_registry.emplace<engine::PlayerControlled>(player);
	m_registry.emplace<engine::Position>(player, sf::Vector2f{10.f, 10.f});
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

	sf::Color tileFillColor = sf::Color(20, 190, 20);
	sf::Color tileOutlineColor = sf::Color(80, 50, 80);

	frame.tileVertices.clear();
	frame.tileOutlines.clear();

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			sf::Vector2f p0 = {(float)x, (float)y};
			sf::Vector2f p1 = {(float)x + 1, (float)y};
			sf::Vector2f p2 = {(float)x + 1, (float)y + 1};
			sf::Vector2f p3 = {(float)x, (float)y + 1};

			sf::Vector2f s0 = camera.worldToScreen(p0);
			sf::Vector2f s1 = camera.worldToScreen(p1);
			sf::Vector2f s2 = camera.worldToScreen(p2);
			sf::Vector2f s3 = camera.worldToScreen(p3);

			frame.tileVertices.push_back({s0, tileFillColor});
			frame.tileVertices.push_back({s1, tileFillColor});
			frame.tileVertices.push_back({s3, tileFillColor});
			frame.tileVertices.push_back({s1, tileFillColor});
			frame.tileVertices.push_back({s2, tileFillColor});
			frame.tileVertices.push_back({s3, tileFillColor});

			frame.tileOutlines.push_back({s0, tileOutlineColor});
			frame.tileOutlines.push_back({s1, tileOutlineColor});
			frame.tileOutlines.push_back({s1, tileOutlineColor});
			frame.tileOutlines.push_back({s2, tileOutlineColor});
			frame.tileOutlines.push_back({s2, tileOutlineColor});
			frame.tileOutlines.push_back({s3, tileOutlineColor});
			frame.tileOutlines.push_back({s3, tileOutlineColor});
			frame.tileOutlines.push_back({s0, tileOutlineColor});
		}
	}

	systems::renderSystem(m_registry, frame, camera, m_engine->texture);
}

bool GameLoop::isFinished() const { return m_finished; }
