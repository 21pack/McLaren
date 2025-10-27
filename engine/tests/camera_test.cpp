#include "camera.h"
#include "gtest/gtest.h"

const float TOLERANCE = 0.0001f;

TEST(CameraTest, WorldToScreen) {
	engine::Camera cam;
	cam.setTileSize(32.f, 32.f);
	cam.zoom = 1.0f;

	sf::Vector2f screenPos0 = cam.worldToScreen({0.f, 0.f});
	EXPECT_NEAR(screenPos0.x, 0.f, TOLERANCE);
	EXPECT_NEAR(screenPos0.y, 0.f, TOLERANCE);

	sf::Vector2f screenPos1 = cam.worldToScreen({1.f, 0.f});
	EXPECT_NEAR(screenPos1.x, 16.f, TOLERANCE);
	EXPECT_NEAR(screenPos1.y, 16.f, TOLERANCE);

	sf::Vector2f screenPos2 = cam.worldToScreen({0.f, 1.f});
	EXPECT_NEAR(screenPos2.x, -16.f, TOLERANCE);
	EXPECT_NEAR(screenPos2.y, 16.f, TOLERANCE);

	sf::Vector2f screenPos3 = cam.worldToScreen({1.f, 1.f});
	EXPECT_NEAR(screenPos3.x, 0.f, TOLERANCE);
	EXPECT_NEAR(screenPos3.y, 32.f, TOLERANCE);
}

TEST(CameraTest, ScreenToWorld) {
	engine::Camera cam;
	cam.setTileSize(32.f, 32.f);
	cam.zoom = 1.0f;

	sf::Vector2f worldPos0 = cam.screenToWorld({0.f, 0.f});
	EXPECT_NEAR(worldPos0.x, 0.f, TOLERANCE);
	EXPECT_NEAR(worldPos0.y, 0.f, TOLERANCE);

	sf::Vector2f worldPos1 = cam.screenToWorld({16.f, 16.f});
	EXPECT_NEAR(worldPos1.x, 1.f, TOLERANCE);
	EXPECT_NEAR(worldPos1.y, 0.f, TOLERANCE);

	sf::Vector2f worldPos2 = cam.screenToWorld({-16.f, 16.f});
	EXPECT_NEAR(worldPos2.x, 0.f, TOLERANCE);
	EXPECT_NEAR(worldPos2.y, 1.f, TOLERANCE);
}

TEST(CameraTest, RoundTrip) {
	engine::Camera cam;
	cam.setTileSize(32.f, 32.f);
	cam.zoom = 1.0f;

	sf::Vector2f worldOriginal = {12.5f, -3.2f};

	sf::Vector2f screen = cam.worldToScreen(worldOriginal);
	sf::Vector2f worldResult = cam.screenToWorld(screen);

	EXPECT_NEAR(worldOriginal.x, worldResult.x, TOLERANCE);
	EXPECT_NEAR(worldOriginal.y, worldResult.y, TOLERANCE);
}
