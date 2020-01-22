#pragma once
#include "Graphics.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class ScreenArea
{
private:
	Vec2 origin;
	RenderWindow& mWindow;
	Transform transf;
public:
	ScreenArea(RenderWindow& mWindow, Vec2 origin)
		:
		mWindow(mWindow),
		origin(origin)
	{
		transf.translate(origin);
	}

	void draw(Drawable& t) const {
		mWindow.draw(t, transf);
	}

	Vec2 getOrigin() {
		return origin;
	}
};

