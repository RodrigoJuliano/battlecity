#pragma once

#include "Ground.h"
#include "Entity.h"
#include "Explosion.h"

class Bullet : public Entity {
private:
	bool collided = false;
	bool canBreakSteel;
public:
	Bullet(Texture& tex, IntRect texRect, Vec2 velocity, bool canBreakSteel = false);
	bool CollidesWith(int block) const;
	void update(float dt, Ground& grnd);
	bool Collided() const;
};