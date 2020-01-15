#pragma once

#include "Ground.h"
#include "Entity.h"

class Bullet : public Entity {
private:
	bool collided = false;
public:
	Bullet(Texture& tex, IntRect texRect, Vec2 pos);
	bool CollidesWith(const Block* b) const;
	void update(float dt, Ground& grnd);
	bool Collided();
	FloatRect getCollisionBox() const;
};