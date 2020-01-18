#pragma once

#include <SFML/Graphics.hpp>
#include "Ground.h"
#include "Tank.h"
#include "Bullet.h"
#include "ScreenArea.h"
#include <list>

using namespace std;
using namespace sf;

class Game
{
private:
	RenderWindow& mWindow;
	Ground grnd;
	Texture texture;
	int id = 0;
	Tank* player;
	bool toggleBlockPressed = false;
	bool firePressed = false;
	list<Entity*> entities;
	ScreenArea area_grnd;
	// screen edges
	RectangleShape edges;
public:
	Game(RenderWindow& mWindow);
	void update(float dt);
	void draw();
};

