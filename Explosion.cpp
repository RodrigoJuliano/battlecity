#include "Explosion.h"

Explosion::Explosion(Texture& tex, IntRect firstframe, int nFrames,
	float frameTime)
	:
	cSprite(tex, firstframe, nFrames, 0.f, frameTime)
{

}

void Explosion::update(float dt) {
	if (curFrame == (nFrames - 1) && curframetime > frameTime)
		finished = true;
	else
		cSprite::update(dt);
}

bool Explosion::finishedAnim() {
	return finished;
}
