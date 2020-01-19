#include "Enemy.h"

Enemy::Enemy(int id, Texture& tex, IntRect firstframe, mt19937& rng)
	:
	Tank(id, tex, firstframe),
	rng(rng),
	onColDirDis(0,30),
	anyTimeDirDis(0.f, 1.f),
	fireDis(0,100)
{
}

void Enemy::update(float dt, Ground& grnd)
{
	Tank::update(dt, grnd);

	Vec2 vel = GetVel();

	if (abs(anyTimeDirDis(rng)) > 3.f) {
		swap(vel.x, vel.y);
	}

	setVel(vel);
}

bool Enemy::tryFire()
{
	if (fireDis(rng) == 0)
		return Tank::tryFire();
	return false;
}

void Enemy::onCollidUp()
{
	Vec2 vel = GetVel();
	int newdir = onColDirDis(rng);
	float velc = max(abs(vel.x), abs(vel.y));

	switch (newdir)
	{
	case 0: // go down
		vel = { 0.f, velc };
		break;
	case 1: // go left
		vel = { -velc, 0.f };
		break;
	case 2: // go rigth
		vel = { velc, 0.f };
		break;
	}

	setVel(vel);
}

void Enemy::onCollidDown()
{
	Vec2 vel = GetVel();
	int newdir = onColDirDis(rng);
	float velc = max(abs(vel.x), abs(vel.y));

	switch (newdir)
	{
	case 0: // go up
		vel = { 0.f, -velc };
		break;
	case 1: // go left
		vel = { -velc, 0.f };
		break;
	case 2: // go rigth
		vel = { velc, 0.f };
		break;
	}

	setVel(vel);
}

void Enemy::onCollidLeft()
{
	Vec2 vel = GetVel();
	int newdir = onColDirDis(rng);
	float velc = max(abs(vel.x), abs(vel.y));

	switch (newdir)
	{
	case 0: // go up
		vel = { 0.f, -velc };
		break;
	case 1: // go down
		vel = { 0.f, velc };
		break;
	case 2: // go rigth
		vel = { velc, 0.f };
		break;
	}

	setVel(vel);
}

void Enemy::onCollidRigth()
{
	Vec2 vel = GetVel();
	int newdir = onColDirDis(rng);
	float velc = max(abs(vel.x), abs(vel.y));

	switch (newdir)
	{
	case 0: // go up
		vel = { 0.f, -velc };
		break;
	case 1: // go down
		vel = { 0.f, velc };
		break;
	case 2: // go left
		vel = { -velc, 0.f };
		break;
	}

	setVel(vel);
}

void Enemy::setExploded()
{
	exploded = true;
}

bool Enemy::getExploded()
{
	return exploded;
}
