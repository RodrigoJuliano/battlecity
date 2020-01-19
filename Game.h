#pragma once

#include <SFML/Graphics.hpp>
#include "Ground.h"
#include "Tank.h"
#include "Bullet.h"
#include "ScreenArea.h"
#include "Enemy.h"
#include <list>
#include <random>

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
	bool spawnEnPressed = false;
	list<Enemy*> enemies;
	list<std::pair<Bullet*, Tank*>> bullets;
	list<Explosion*> explosions;
	ScreenArea area_grnd;
	// screen edges
	RectangleShape edges;
	mt19937 rng;
	random_device rd;
public:
	Game(RenderWindow& mWindow);
	void update(float dt);
	void draw();
};

