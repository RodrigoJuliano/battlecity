#include "ScreenArea.h"

ScreenArea::ScreenArea(RenderWindow& mWindow, Vec2 origin)
	:
	mWindow(mWindow),
	origin(origin)
{
	transf.translate(origin);
}

void ScreenArea::draw(Drawable& t) const {
	mWindow.draw(t, transf);
}

Vec2 ScreenArea::getOrigin() {
	return origin;
}
