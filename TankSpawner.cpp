#include "TankSpawner.h"

TankSpawner::TankSpawner(Texture& tex, Vec2 pos, float time2spawn)
	:
	cSprite(tex, { 64, 112, 16, 14 }, 4, 0.f, 0.1f),
	time2spawn(time2spawn)
{
	setPosition(pos);
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

Tank* TankSpawner::spawnTank() {
	tank->setPosition(getPosition());
	Tank* t = tank;
	tank = nullptr;
	return t;
}

void TankSpawner::reset() {
	curTime = 0.f;
}

bool TankSpawner::isSpawning() const
{
	return tank;
}

void TankSpawner::startSpawn(Tank* tank)
{
	this->tank = tank;
	reset();
}
