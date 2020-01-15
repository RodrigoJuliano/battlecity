#pragma once

#include <SFML/Graphics.hpp>
#include "Ground.h"
#include "Tank.h"
#include "Bullet.h"

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
	vector<Bullet*> bullets;
public:
	Game(RenderWindow& mWindow);
	void update(float dt);
	void draw();
	void loadBlocks();
};

