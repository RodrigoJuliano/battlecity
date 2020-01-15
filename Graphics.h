#pragma once

#include <SFML/Graphics.hpp>

typedef sf::Vector2f Vec2;
typedef sf::Vector2i Vei2;

namespace Gfx {
	const int ScreenWidth = 620;
	const int ScreenHeight = 620;
	const int TextureResolution = 8;
	const float TextureScaleMult = 2.5f;
	const int EdgeSize = 50;

	inline Vec2 clamp2Screen(const Vec2& pos) {
		Vec2 np = pos;
		if (pos.x < 16)
			np.x = 16;
		else if (pos.x > ScreenWidth-16)
			np.x = ScreenWidth-16;
		if (pos.y < 16)
			np.y = 16;
		else if (pos.y > ScreenHeight-16)
			np.y = ScreenHeight-16;
		return np;
	}
}