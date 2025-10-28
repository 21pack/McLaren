#pragma once

#include "input.h"
#include <memory>

namespace engine {

struct RenderFrame;
struct Camera;
struct Engine;
struct World;

class ILoop {
  public:
	virtual ~ILoop() = default;

	virtual void init(Engine &engine) {}
	virtual void update(Input &input, float dt) = 0; // Updating scene logic
	virtual void collectRenderData(RenderFrame &frame,
								   Camera &camera) = 0; // Rendering the scene
	virtual bool isFinished() const = 0; // Check if the scene is finished
	virtual void exit() {
		m_finished = true;
	} // Force scene termination (on Engine request)

  protected:
	bool m_finished = false;
};
using LoopPtr = std::unique_ptr<ILoop>;

} // namespace engine
