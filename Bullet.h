#pragma once

#include "Ground.h"
#include "Entity.h"
#include "Explosion.h"

class Bullet : public Entity {
private:
	bool collided = false;
public:
	Bullet(Texture& tex, IntRect texRect, Vec2 velocity);
	bool CollidesWith(int block) const;
	void update(float dt, Ground& grnd);
	bool Collided() const;
};