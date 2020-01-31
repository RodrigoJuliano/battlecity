#pragma once
#include "cSprite.h"
#include "Tank.h"

class TankSpawner : public cSprite
{
private:
	const float time2spawn;
	float curTime = 0.f;
	Tank* tank = nullptr;
public:
	TankSpawner(Texture& tex, Vec2 pos, float time2spawn = 1.2f);
	void update(float dt);
	bool mustSpawn() const;
	Tank* spawnTank();
	void reset();
	bool isSpawning() const;
	void startSpawn(Tank* tank);
};

