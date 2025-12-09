#pragma once

#include "core/loop.h"
#include "ecs/tile.h"
#include "resources/serializable_world.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <entt/entt.hpp>
#include <vector>

namespace engine {
struct Engine;
struct Camera;
struct RenderFrame;
} // namespace engine

/**
 * @brief Main game loop implementation for gameplay scene.
 *
 * Handles game logic updates, entity management, and rendering data collection
 * for the primary gameplay state. Manages all game objects and tile-based world.
 */
class GameLoop : public engine::ILoop {
  public:
	/**
	 * @brief Constructs game loop and loads world data from JSON file.
	 *
	 * Initializes tile map and textures from serialized world data file.
	 * Loads world layout, dimensions, and tile textures from JSON configuration.
	 */
	GameLoop();
	virtual ~GameLoop() = default;

	/**
	 * @brief Initializes the game state and creates all entities.
	 *
	 * Sets up the game world, creates static map geometry, spawns player character,
	 * NPCs, and configures the camera. Loads textures and generates render data.
	 */
	void init() override;

	/**
	 * @brief Updates game logic for the current frame.
	 * @param input Reference to the input system for player controls.
	 * @param dt Delta time in seconds since last update.
	 *
	 * Processes player input, movement, animations, and camera tracking.
	 * Executes all game systems in defined order.
	 */
	void update(engine::Input &input, float dt) override;

	/**
	 * @brief Collects all render data for the current frame.
	 * @param frame Reference to the render frame to populate with draw commands.
	 * @param camera Reference to the camera for view-relative rendering.
	 *
	 * Gathers static map geometry, entity sprites, and visual elements for
	 * rendering. Combines pre-computed tile vertices with dynamic entity render
	 * data.
	 */
	void collectRenderData(engine::RenderFrame &frame,
						   engine::Camera &camera) override;

	/**
	 * @brief Checks if the game loop should terminate.
	 * @return True if the game loop has finished execution, false otherwise.
	 */
	bool isFinished() const override;

  private:
	/**
	 * @brief Container managing all game objects, their components and
	 * relationships.
	 *
	 * The registry stores entities (game objects) and their components (data),
	 * providing efficient querying and management of object relationships.
	 * It serves as the database for all dynamic game elements.
	 */
	entt::registry m_registry;

	engine::Engine *m_engine = nullptr; ///< Pointer to the main engine instance

	/**
	 * @brief Updates animation states based on entity movement.
	 * @param dt Delta time in seconds for animation timing.
	 *
	 * Transitions between idle and run animations depending on entity velocity.
	 * Manages animation state changes and frame resetting.
	 */
	void gameAnimationSystem(float dt);

	int width;	///< World width in tile units
	int height; ///< World height in tile units
	std::unordered_map<int, engine::TileTexture>
		tileTextures;						   ///< Tile ID to texture data mapping
	std::vector<sf::VertexArray> m_tileMeshes; ///< Pre-computed vertex data for
											   ///< static ground layer rendering
	std::vector<engine::Tile>
		tiles; ///< Tile data representing world layout, collision, and layers

	sf::VertexArray m_visibleTileVertices;
	sf::Vector2i m_worldSize;

	sf::Vector2f m_lastCameraPosition;
	float m_cameraMoveThreshold = 10.0f;
};
