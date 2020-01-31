#pragma once

#include "Gfx.h"
#include "Vec2.h"
#include "cSprite.h"
#include "Ground.h"

using namespace std;

class Entity : public cSprite
{
protected:
	Vec2 velocity;
public:
	Entity(Texture& tex, IntRect firstframe, int nFrames, float collisionsize, float frameTime = 0.1f);
	Vec2 GetVel() const;
	void setVel(Vec2 vel);
	Vec2 GetDirection() const;
	virtual bool CollidesWith(int block) const;
	virtual void update(float dt, Ground& grnd);
};