#pragma once

#include "Entity.h"
#include "Ground.h"

class Explosion : public Entity
{
private:
	bool finished = false;
public:
	Explosion(Texture& tex, IntRect firstframe, int nFrames);
	void update(float dt, Ground& grnd);
	bool finishedAnim();
};

