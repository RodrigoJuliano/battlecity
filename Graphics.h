#pragma once

#include <SFML/Graphics.hpp>

typedef sf::Vector2f Vec2;
typedef sf::Vector2i Vei2;

namespace Gfx {
	const int ScreenWidth = 640;
	const int ScreenHeight = 600;
	const int TextureResolution = 8;
	const float TextureScaleMult = 2.5f;
	const int EdgeSize = 40;
	const int TileSize = TextureResolution * TextureScaleMult;
}

enum class Screen {
	startScreen,
	selectStage,
	nextStage,
	construct,
	pauseScreen,
	playScreen,
	gameOver
};