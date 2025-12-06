#include "core/render.h"

#include "core/camera.h"
#include "core/loop.h"
#include "ecs/tile.h"
#include <cmath>

namespace engine {

std::shared_ptr<RenderFrame> Render::collectFrame(ILoop &loop, Camera &camera) {
	auto frame = std::make_shared<RenderFrame>();

	frame->clearColor = sf::Color::Black;
	frame->cameraView = sf::View(sf::FloatRect(sf::Vector2f(0, 0), camera.size));
	frame->cameraView.setCenter(camera.position);

	loop.collectRenderData(*frame, camera);

	return frame;
}

void Render::drawSprite(sf::RenderWindow &window,
						const RenderFrame::SpriteData &sprite, int step) {
	window.draw(sprite.shadowVertices);
	const auto &rect = sprite.textureRect;
	int texW = rect.size.x;
	int texH = rect.size.y;

	// Pre-calculation of transformation
	const float angle = sprite.rotation.asRadians();
	const float cosA = std::cos(angle);
	const float sinA = std::sin(angle);

	const sf::Image &img = *sprite.image;

	std::vector<sf::Vertex> vertices;
	vertices.reserve((texW / step) * (texH / step));

	float zoom = 2.f;
	int pointSize = static_cast<int>(std::ceil(zoom));

	for (int ty = 0; ty < texH; ty += step) {
		for (int tx = 0; tx < texW; tx += step) {
			// Local pixel coordinates
			float localX = static_cast<float>(tx) * sprite.scale.x;
			float localY = static_cast<float>(ty) * sprite.scale.y;

			// Rotate
			float rotatedX = localX * cosA - localY * sinA;
			float rotatedY = localX * sinA + localY * cosA;

			// Position in world coordinates
			float worldX = sprite.position.x + rotatedX;
			float worldY = sprite.position.y + rotatedY;

			// Texture sampling
			int u = rect.position.x + tx;
			int v = rect.position.y + ty;

			if (u < 0 || v < 0 || u >= static_cast<int>(img.getSize().x) ||
				v >= static_cast<int>(img.getSize().y))
				continue;

			sf::Color texColor = img.getPixel({(unsigned int)u, (unsigned int)v});

			// Animation with sprite color
			sf::Color finalColor(texColor.r * sprite.color.r / 255,
								 texColor.g * sprite.color.g / 255,
								 texColor.b * sprite.color.b / 255,
								 texColor.a * sprite.color.a / 255);

			for (int dy = 0; dy < pointSize; ++dy) {
				for (int dx = 0; dx < pointSize; ++dx) {
					sf::Vertex vertex;
					vertex.position = sf::Vector2f(worldX + dx, worldY + dy);
					vertex.color = finalColor;
					vertices.push_back(vertex);
				}
			}
		}
	}

	if (!vertices.empty())
		window.draw(vertices.data(), static_cast<size_t>(vertices.size()),
					sf::PrimitiveType::Points);
}

void Render::generateTileMapVertices(
	std::vector<sf::VertexArray> &tileMeshes, Camera &camera,
	const std::vector<Tile> &tiles, int worldWidth, int worldHeight,
	std::unordered_map<int, engine::TileData> &tileImages) {
	sf::Vector2f tileSize = camera.getTileSize();
	float tileWidth = tileSize.x;
	float tileHeight = tileSize.y * 2.f;

	camera.setTileSize(tileWidth, tileHeight / 2);

	const int step = 1; // If want to make draw faster, you can increase it
	float zoom = camera.zoom;
	int pointSize = static_cast<int>(std::ceil(zoom));

	tileMeshes.clear();
	tileMeshes.resize(worldWidth * worldHeight);

	auto getIndex = [&](int x, int y) { return y * worldWidth + x; };

	for (int y = 0; y < worldHeight; ++y) {
		for (int x = 0; x < worldWidth; ++x) {
			int index = getIndex(x, y);

			sf::VertexArray &mesh = tileMeshes[index];
			mesh.setPrimitiveType(sf::PrimitiveType::Points);
			mesh.clear();

			const auto &tile = tiles[index];
			sf::Vector2f isoVec = camera.worldToScreen({(float)x, (float)y});

			for (int layerId : tile.layerIds) {
				if (tileImages.find(layerId) == tileImages.end()) {
					continue;
				}

				const TileData &tileData = tileImages[layerId];
				sf::Image &tileImage = *tileData.image;
				int layerHeight = tileData.height;

				for (int ty = 0; ty < tileHeight; ty += step) {
					for (int tx = 0; tx < tileWidth; tx += step) {
						if (tx < 0 || ty < 0 || tx >= tileWidth || ty >= tileHeight)
							continue;

						sf::Color color =
							tileImage.getPixel({(unsigned)tx, (unsigned)ty});
						if (color.a == 0)
							continue;

						float pixelX = isoVec.x + tx * zoom;
						float pixelY = isoVec.y + ty * zoom - layerHeight * zoom;

						for (int dy = 0; dy < pointSize; ++dy) {
							for (int dx = 0; dx < pointSize; ++dx) {
								mesh.append({{pixelX + dx, pixelY + dy}, color});
							}
						}
					}
				}
			}
		}
	}
}

void Render::renderMap(std::vector<sf::VertexArray> &tileMeshes, Camera &camera,
					   const sf::Vector2i wordSize, sf::VertexArray &tileVertices) {
	tileVertices.clear();
	tileVertices.setPrimitiveType(sf::PrimitiveType::Points);

	sf::FloatRect cameraBounds = camera.getBounds();

	sf::Vector2f rawTileSize = camera.getTileSize();

	// Constants (10.f) is needed to account for tile overlapping
	float scaledTileW = rawTileSize.x * camera.zoom + 10.f;
	float scaledTileH = rawTileSize.y * 2.f * camera.zoom + 10.f;

	for (int y = 0; y < wordSize.y; ++y) {
		for (int x = 0; x < wordSize.x; ++x) {
			sf::Vector2f tilePos = camera.worldToScreen({(float)x, (float)y});

			sf::FloatRect tileBounds({tilePos.x, tilePos.y - scaledTileH},
									 {scaledTileW, scaledTileH * 2.f});

			if (tileBounds.findIntersection(cameraBounds).has_value()) {
				int index = y * wordSize.x + x;

				if (index < tileMeshes.size()) {
					const sf::VertexArray &cachedMesh = tileMeshes[index];

					for (std::size_t i = 0; i < cachedMesh.getVertexCount(); ++i) {
						tileVertices.append(cachedMesh[i]);
					}
				}
			}
		}
	}
}

void Render::drawFrame(const RenderFrame &frame) {
	window.clear(frame.clearColor);
	window.setView(frame.cameraView);

	// Draw map
	window.draw(frame.tileVertices);

	// Draw sprites
	for (auto &spr : frame.sprites) {
		drawSprite(window, spr, 1);
	}
}
} // namespace engine
