namespace engine {

struct EntityId {
	uint32_t index;
	uint32_t generation;
};

struct Entity {
	EntityId id;				// Unique identifier
	World *world = nullptr;		// Reference to the world the object is in
	sf::Vector2f position;		// Position of the object in the world
	float direction = 0.f;		// Direction
	int frameIdx = 0;			// Current animation frame index
	int frameCount = 1;			// Total number of frames in the animation
	float frameTime = 0.f;		// Timer for switching frames
	float frameDuration = 0.1f; // Time between frames

	virtual void update(float dt); // Object logic (movement, animation)
	virtual void
	render(Renderer &,
		   const Camera &) = 0; // Rendering (must take camera into account)
};

} // namespace engine
