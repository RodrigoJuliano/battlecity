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
	ScreenArea(RenderWindow& mWindow, Vec2 origin);
	void draw(Drawable& t) const;
	Vec2 getOrigin();
};

