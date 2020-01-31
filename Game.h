#pragma once

#include "Ground.h"
#include "Tank.h"
#include "Bullet.h"
#include "ScreenArea.h"
#include "Enemy.h"
#include "SoundSystem.h"
#include "Bonus.h"
#include "Player.h"
#include "Hud.h"
#include "TankSpawner.h"
#include <SFML/Graphics.hpp>
#include <list>
#include <random>

using namespace std;
using namespace sf;

class Game
{
private:
	RenderWindow& mWindow;
	Texture texture;
	SoundSystem soundSys;
	// random
	mt19937 rng;
	random_device rd;
	// Screen and hud
	ScreenArea area_grnd;
	ScreenArea area_hud;
	Hud hud;
	Screen curScreen;
	// Map
	Ground grnd;
	bool customMap = false;
	cSprite falcon;
	// Player
	Player* player;
	Vec2 pSpawnPos;
	TankSpawner* pSpawner = nullptr;
	bool pmovesound = false;
	// enemies
	list<Enemy*> enemies;
	const int totalEnemies = 20;
	const int maxEnemies = 4;
	int totalSpawnedEnemies = 0;
	uniform_int_distribution<int> enemyPosDist;
	uniform_int_distribution<int> enemySpawnDist;
	uniform_int_distribution<int> enemyBonusMarkDist;
	list<TankSpawner*> enemy_spawners;
	// number of enemies of each type
	int nBasicTank = 0;
	int nSpeedTank = 0;
	int nPowerTank = 0;
	int nArmorTank = 0;
	// bonus system
	Bonus* bonus = nullptr;
	uniform_int_distribution<int> bonusTypeDist;
	normal_distribution<float> bonusPosDist;
	bool timerBonusOn = false;
	float timerBonusTime = 0.f;
	bool shovelBonusOn = false;
	float shovelBonusTime = 0.f;
	// other
	list<std::pair<Bullet*, Tank*>> bullets;
	list<Explosion*> explosions;
private:
	void ctrlNumEnemies();
	void setblocksshovelbonus(int block);
	void spawnBonus();
	void loadLevel(int level);
	void resetPlayer();
	void resetGame();
public:
	Game(RenderWindow& mWindow);
	void update(float dt);
	void draw();
	~Game();
};

