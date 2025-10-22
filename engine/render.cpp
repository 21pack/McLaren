#include "render.h"

namespace engine {

std::shared_ptr<RenderFrame> Render::collectFrame(ILoop &loop, Camera &camera) {
	auto frame = std::make_shared<RenderFrame>();

	frame->clearColor = sf::Color::Black;
	frame->cameraView = sf::View(sf::FloatRect(
		sf::Vector2f(0, 0), sf::Vector2f(window.getSize().x, window.getSize().y)));
	frame->cameraView.setCenter(camera.position);

	loop.collectRenderData(*frame, camera);

	return frame;
}

void Render::drawFrame(const RenderFrame &frame) {
	window.clear(frame.clearColor);
	window.setView(frame.cameraView);

	// Draw tiles
	// TODO: draw it in init and save
	window.draw(frame.tileVertices.data(), frame.tileVertices.size(),
				sf::PrimitiveType::Triangles);
	window.draw(frame.tileOutlines.data(), frame.tileOutlines.size(),
				sf::PrimitiveType::Lines);

	// Draw sprites
	for (auto &s : frame.sprites) {
		sf::Sprite sprite = sf::Sprite(*s.texture);
		sprite.setTextureRect(s.textureRect);
		sprite.setPosition(s.position);
		sprite.setRotation(s.rotation);
		sprite.setScale(s.scale);
		sprite.setColor(s.color);

		window.draw(sprite);
	}
}

} // namespace engine
