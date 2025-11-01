#pragma once

#include "input.h"
#include <memory>

namespace engine {

struct RenderFrame;
struct Camera;
struct Engine;
struct World;

/**
 * @brief Interface for game loops and scenes.
 *
 * Defines the contract for different game states.
 * Each loop manages its own update logic, rendering, and lifecycle.
 */
class ILoop {
  public:
	virtual ~ILoop() = default;

	/**
	 * @brief Initializes the loop state.
	 * Called once when the loop becomes active.
	 */
	virtual void init() {}

	/**
	 * @brief Updates the scene logic for the current frame.
	 * @param input Reference to the input system for user interaction.
	 * @param dt Delta time in seconds since the last update.
	 */
	virtual void update(Input &input, float dt) = 0;

	/**
	 * @brief Collects render data for the current frame.
	 * @param frame Reference to the render frame for collecting draw commands.
	 * @param camera Reference to the camera for view-dependent rendering.
	 */
	virtual void collectRenderData(RenderFrame &frame, Camera &camera) = 0;

	/**
	 * @brief Checks if the loop has finished execution.
	 * @return True if the loop should terminate, false otherwise.
	 */
	virtual bool isFinished() const = 0;

	/**
	 * @brief Forces termination of the loop.
	 * Called by the engine when switching to a different loop.
	 */
	virtual void exit() { m_finished = true; }

  protected:
	bool m_finished = false; ///< Internal flag tracking loop completion state
};

using LoopPtr = std::unique_ptr<ILoop>; ///< Smart pointer type for loop management

} // namespace engine
