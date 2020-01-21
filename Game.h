#pragma once

#include <SFML/Graphics.hpp>
#include "Ground.h"
#include "Tank.h"
#include "Bullet.h"
#include "ScreenArea.h"
#include "Enemy.h"
#include <list>
#include <random>
#include "SoundSystem.h"

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
	const int totalEnemies = 20;
	const int maxEnemies = 4;
	int spawnedEnemies = 0;
	uniform_int_distribution<int> enemyPosDist;
	uniform_int_distribution<int> enemySpawnDist;
	uniform_int_distribution<int> enemyTypeDist;
	Vec2 pSpawnPos;
	SoundSystem soundSys;
	bool pmovesound = false;
private:
	void ctrlNumEnemies();
public:
	Game(RenderWindow& mWindow);
	void update(float dt);
	void draw();
};

