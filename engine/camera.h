namespace engine {

struct Camera {
	sf::Vector2f position; // Camera coordinates in the world
	float zoom = 1.0f;	   // TODO: needs?

	sf::Vector2f worldToScreen(sf::Vector2f worldPos)
		const; // Converts world coordinates to screen coordinates (isometry)

	sf::Vector2f
	screenToWorld(const sf::Vector2f &screenPos) const; // Inverse transformation
};

} // namespace engine
