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
	TankSpawner(Texture& tex, Tank* tank, float time2spawn, Vec2 pos)
		:
		cSprite(tex, {64, 112, 16, 14}, 4, 0.f, 0.1f),
		tank(tank),
		time2spawn(time2spawn)
	{
		setPosition(pos);
		tank->setPosition(pos);
	}

	void update(float dt) {
		curTime += dt;
		cSprite::update(dt);
	}

	bool mustSpawn() const {
		if (curTime > time2spawn) {
			return true;
		}
		return false;
	}

	Tank* getTank() const{
		return tank;
	}

	void reset() {
		curTime = 0.f;
	}
};

