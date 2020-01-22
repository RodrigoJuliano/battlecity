#pragma once
#include "Entity.h"
#include "Ground.h"
#include "Graphics.h"
#include "Bullet.h"

class Tank : public Entity
{
private:
	const int maxFire = 1;
	int fireCounter = 0;
	int health;
public:
	Tank(Texture& tex, IntRect firstframe, int health = 1);
	virtual void update(float dt, Ground& grnd);
	virtual bool tryFire();
	void decFireCount();
	int getFireCount();
	virtual void onCollidUp() {};
	virtual void onCollidDown() {};
	virtual void onCollidLeft() {};
	virtual void onCollidRigth() {};
	void hit();
	int getHealth();
	void kill();
};

