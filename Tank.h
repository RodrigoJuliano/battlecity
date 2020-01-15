#pragma once
#include "Entity.h"
#include "Ground.h"
#include "Graphics.h"

class Tank : public Entity
{
private:
	int fireInterval = 20;
	int fireCounter = 0;
public:
	Tank(int id, Texture& tex, IntRect firstframe, Vec2 pos);
	void update(float dt, const Ground& grnd);
};

