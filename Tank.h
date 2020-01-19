#pragma once
#include "Entity.h"
#include "Ground.h"
#include "Graphics.h"
#include "Bullet.h"

class Tank : public Entity
{
private:
	int maxFire = 1;
	int fireCounter = 0;
public:
	Tank(int id, Texture& tex, IntRect firstframe);
	virtual void update(float dt, Ground& grnd);
	virtual bool tryFire();
	void decFireCount();
	int getFireCount();
	virtual void onCollidUp() {};
	virtual void onCollidDown() {};
	virtual void onCollidLeft() {};
	virtual void onCollidRigth() {};
};

