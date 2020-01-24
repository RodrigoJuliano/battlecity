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
#include "Bonus.h"
#include "Player.h"
#include "Hud.h"
#include "TankSpawner.h"

using namespace std;
using namespace sf;

class Game
{
private:
	RenderWindow& mWindow;
	Ground grnd;
	Texture texture;
	int id = 0;
	Player* player;
	bool toggleBlockPressed = false;
	bool firePressed = false;
	bool spawnEnPressed = false;
	list<Enemy*> enemies;
	list<std::pair<Bullet*, Tank*>> bullets;
	list<Explosion*> explosions;
	ScreenArea area_grnd;
	ScreenArea area_hud;
	mt19937 rng;
	random_device rd;
	const int totalEnemies = 20;
	const int maxEnemies = 4;
	int spawnedEnemies = 0;
	uniform_int_distribution<int> enemyPosDist;
	uniform_int_distribution<int> enemySpawnDist;
	Vec2 pSpawnPos;
	SoundSystem soundSys;
	bool pmovesound = false;
	Bonus* bonus = nullptr;
	uniform_int_distribution<int> enemyBonusMarkDist;
	uniform_int_distribution<int> bonusTypeDist;
	normal_distribution<float> bonusPosDist;
	bool timerBonusOn = false;
	float timerBonusTime = 0.f;
	bool shovelBonusOn = false;
	float shovelBonusTime = 0.f;
	Hud hud;
	list<TankSpawner*> spawners;
	TankSpawner* pSpawner = nullptr;
	Screen curScreen;
	bool startPressed = false;
	bool selectPressed = false;
	bool aPressed = false;
	bool bPressed = false;
	// number of enemies of each type
	int nBasicTank = 0;
	int nSpeedTank = 0;
	int nPowerTank = 0;
	int nArmorTank = 0;
	bool customMap = false;
	const int numStages = 1;
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

