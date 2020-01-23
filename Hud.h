#pragma once

#include <SFML/Graphics.hpp>
#include "cSprite.h"
#include "TileMap.h"
#include "Counter.h"

using namespace sf;

class Hud : public Drawable
{
private:
	Texture& tex;
	RectangleShape edges1;
	RectangleShape hudback;
	cSprite lifesholder;
	cSprite levelholder;
	Counter lifes;
	Counter level;
	// Enemy counter
	TileMap enemCounter;
	int nEnemies;
public:
	Hud(Texture& tex);
	void setLifes(int lifes);
	void setLevel(int level);
	void removeEnemy();
	void resetEnemCounter();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

