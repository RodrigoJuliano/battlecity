#include "TankSpawner.h"

TankSpawner::TankSpawner(Texture& tex, Tank* tank, float time2spawn, Vec2 pos)
	:
	cSprite(tex, { 64, 112, 16, 14 }, 4, 0.f, 0.1f),
	tank(tank),
	time2spawn(time2spawn)
{
	setPosition(pos);
	tank->setPosition(pos);
}

void TankSpawner::update(float dt) {
	curTime += dt;
	cSprite::update(dt);
}

bool TankSpawner::mustSpawn() const {
	if (curTime > time2spawn) {
		return true;
	}
	return false;
}

Tank* TankSpawner::getTank() const {
	return tank;
}

void TankSpawner::reset() {
	curTime = 0.f;
}
