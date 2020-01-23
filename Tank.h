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
	int health;
	float bulletSpeed;
public:
	Tank(Texture& tex, IntRect firstframe, int health, float bulletSpeed);
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
	float getBulletSpeed();
	void setBulletSpeed(float speed);
	void setMaxFire(int mfire);
	void setHealth(int health);
};

