#pragma once

#include "components.h"
#include "tile.h"
#include <entt/entt.hpp>

namespace engine {
struct Input;
struct RenderFrame;
struct Camera;
struct ImageManager;
} // namespace engine

namespace systems {

/**
 * @brief Processes player input and updates player-controlled entities.
 * @param registry Reference to the ECS registry.
 * @param input Reference to the input system for reading user commands.
 */
void playerInputSystem(entt::registry &registry, const engine::Input &input);

/**
 * @brief Updates entity positions based on velocity and handles tile collision.
 * @param registry Reference to the ECS registry.
 * @param tiles Reference to the world tile data for collision detection.
 * @param worldWidth Width of the world in tiles.
 * @param worldHeight Height of the world in tiles.
 * @param dt Delta time in seconds since last update.
 */
void movementSystem(entt::registry &registry, std::vector<engine::Tile> &tiles,
					int worldWidth, int worldHeight, float dt);

/**
 * @brief Updates animation states and advances animation frames.
 * @param registry Reference to the ECS registry.
 * @param dt Delta time in seconds for frame timing.
 */
void animationSystem(entt::registry &registry, float dt);

/**
 * @brief Collects render data for all visible entities in the current frame.
 * @param registry Reference to the ECS registry.
 * @param frame Reference to the render frame for collecting draw commands.
 * @param camera Reference to the camera for view culling.
 * @param imageManager Reference to the image manager for texture access.
 */
void renderSystem(entt::registry &registry, engine::RenderFrame &frame,
				  const engine::Camera &camera, engine::ImageManager &imageManager);

/**
 * @brief Updates NPC entities to follow the player character.
 * @param registry Reference to the ECS registry.
 * @param dt Delta time in seconds for movement calculations.
 */
void npcFollowPlayerSystem(entt::registry &registry, float dt);

/**
 * @brief Implements wandering behavior for NPC entities.
 * @param registry Reference to the ECS registry.
 * @param dt Delta time in seconds for movement calculations.
 */
void npcWanderSystem(entt::registry &registry, float dt);

/**
 * @brief Creates a new NPC entity with specified parameters.
 * @param registry Reference to the ECS registry.
 * @param pos Initial position of the NPC.
 * @param targetSize Render size of the NPC.
 * @param clips Animation clips for the NPC.
 * @param speed Movement speed of the NPC.
 * @return Entity handle for the created NPC.
 */
entt::entity createNPC(entt::registry &registry, const sf::Vector2f &pos,
					   const sf::Vector2f &targetSize,
					   const std::unordered_map<int, engine::AnimationClip> &clips,
					   float speed);

} // namespace systems
