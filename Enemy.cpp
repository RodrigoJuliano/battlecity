#include "Enemy.h"

Enemy::Enemy(Texture& tex, IntRect firstframe, mt19937& rng, int health, float bulletSpeed)
	:
	Tank(tex, firstframe, health, bulletSpeed),
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
		if(fireDis(rng) > 50)
			if (abs(vel.x) > 0.f)
				vel.x *= -1;
			else if (abs(vel.y) > 0.f)
				vel.y *= -1;
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

void Enemy::setBonusMark(bool mark)
{
	bonusMark = mark;
	if (mark)
		nFrames = 3; // add the red frame
	else
		nFrames = 2;
}

bool Enemy::hasBonusMark() const
{
	return bonusMark;
}
