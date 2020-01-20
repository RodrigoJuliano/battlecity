#include "Explosion.h"

Explosion::Explosion(Texture& tex, IntRect firstframe, int nFrames)
	:
	Entity(tex, firstframe, nFrames, 0.f)
{

}

void Explosion::update(float dt, Ground& grnd)
{
	if (curFrame == (nFrames - 1) && curframetime > frameTime)
		finished = true;
	else
		Entity::update(dt, grnd);
}

bool Explosion::finishedAnim()
{
	return finished;
}
