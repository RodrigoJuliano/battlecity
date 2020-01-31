#pragma once
#include "cSprite.h"
#include "Tank.h"

class TankSpawner : public cSprite
{
private:
	const float time2spawn;
	float curTime = 0.f;
	Tank* tank;
public:
	TankSpawner(Texture& tex, Tank* tank, float time2spawn, Vec2 pos);
	void update(float dt);
	bool mustSpawn() const;
	Tank* getTank() const;
	void reset();
};

