#pragma once

#include "cSprite.h"
#include "Ground.h"

class Explosion : public cSprite
{
private:
	bool finished = false;
public:
	Explosion(Texture& tex, IntRect firstframe, int nFrames, float frameTime = 0.05f)
		:
		cSprite(tex, firstframe, nFrames, 0.f, frameTime)
	{

	}

	void update(float dt) {
		if (curFrame == (nFrames - 1) && curframetime > frameTime)
			finished = true;
		else
			cSprite::update(dt);
	}

	bool finishedAnim() {
		return finished;
	}
};

