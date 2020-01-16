#pragma once
#include "Entity.h"
#include "Ground.h"
#include "Graphics.h"
#include "Bullet.h"

class Tank : public Entity
{
private:
	int fireInterval = 20;
	int fireCounter = 0;
	Bullet bullet;
public:
	Tank(int id, Texture& tex, IntRect firstframe);
	void update(float dt, Ground& grnd);
	void fire();
	virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
};

