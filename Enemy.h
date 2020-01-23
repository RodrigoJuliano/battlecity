#pragma once

#include "Tank.h"
#include <random>

class Enemy : public Tank
{
private:
	mt19937& rng;
	uniform_int_distribution<int> onColDirDis;
	normal_distribution<float> anyTimeDirDis;
	uniform_int_distribution<int> fireDis;
	bool bonusMark = false;
public:
	Enemy(Texture& tex, IntRect firstframe, mt19937& rng, int health, float bulletSpeed);
	virtual void update(float dt, Ground& grnd);
	virtual bool tryFire();
	virtual void onCollidUp();
	virtual void onCollidDown();
	virtual void onCollidLeft();
	virtual void onCollidRigth();
	void setBonusMark(bool mark = true);
	bool hasBonusMark() const;
};

