#pragma once

#include "core/camera.h"
#include "core/input.h"
#include "core/loop.h"
#include "core/render.h"
#include "resources/image_manager.h"

namespace engine {

/**
 * @brief Main game engine class.
 *
 * Coordinates all subsystems: rendering, input, camera, image management.
 * Implemented as a singleton with controlled access through static methods.
 *
 * @warning Class does not support copying or assignment.
 */
class Engine {
  public:
	/**
	 * @brief Sets the active game loop.
	 * @param loop Pointer to the game loop object.
	 */
	void setLoop(LoopPtr loop);

	/**
	 * @brief Starts the main application loop.
	 * Begins execution of the main game loop and runs until termination.
	 */
	void run();

	// Delete copy constructor and assignment operator
	Engine(Engine const &) = delete;
	void operator=(Engine const &) = delete;

	/**
	 * @brief Creates or gets engine instance with specified loop.
	 * @param loop Pointer to the game loop object to set as active.
	 * @return Pointer to the Engine singleton instance.
	 */
	static Engine *withLoop(LoopPtr loop);

	/**
	 * @brief Gets the current engine instance.
	 * @return Pointer to the Engine singleton instance.
	 * @note Engine must be initialized first with withLoop().
	 */
	static Engine *get();

	// Public subsystems
	Render render;			   ///< Handles low-level rendering operations
	Input input;			   ///< Input system for user interaction
	Camera camera;			   ///< Camera management
	RenderQueue renderQueue;   ///< Rendering queue
	LoopPtr activeLoop;		   ///< Current active game loop
	ImageManager imageManager; ///< Image loading and management

  private:
	sf::Clock fpsClock; ///< Clock for FPS tracking and timing

	/**
	 * @brief Private constructor for singleton pattern.
	 */
	Engine() {}
};

} // namespace engine
