#pragma once

#include "cSprite.h"
#include "Ground.h"

class Explosion : public cSprite
{
private:
	bool finished = false;
public:
	Explosion(Texture& tex, IntRect firstframe, int nFrames,
		float frameTime = 0.05f);
	void update(float dt);
	bool finishedAnim();
};

