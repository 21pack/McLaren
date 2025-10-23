#include "render.h"

namespace engine {

std::shared_ptr<RenderFrame> Render::collectFrame(ILoop &loop, Camera &camera) {
	auto frame = std::make_shared<RenderFrame>();

	frame->clearColor = sf::Color::Black;
	frame->cameraView = sf::View(
		sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(window.getSize().x / 2.f,
													   window.getSize().y / 2.f)));
	frame->cameraView.setCenter(camera.position);

	loop.collectRenderData(*frame, camera);

	return frame;
}

void Render::drawFrame(const RenderFrame &frame) {
	window.clear(frame.clearColor);
	window.setView(frame.cameraView);

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
