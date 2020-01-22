#include "Bullet.h"

Bullet::Bullet(Texture& tex, IntRect texRect, Vec2 velocity, bool canBreakSteel)
	:
	Entity(tex, texRect, 1, 14.f),
	canBreakSteel(canBreakSteel)
{
	setVel(velocity);
}

bool Bullet::CollidesWith(int block) const
{
	return Entity::CollidesWith(block) && block != 12;
}

void Bullet::update(float dt, Ground& grnd)
{
	Entity::update(dt, grnd);

	// Do the entity move checking for collision with map

	Vec2 p = getPosition();
	p.x += velocity.x * dt;
	p.y += velocity.y * dt;
	setPosition(p);

	auto r = getCollisionBox();
	Vec2 p1 = { r.left, r.top };
	Vec2 p2 = { r.left, r.top + r.height };
	Vec2 p3 = { r.left + r.width, r.top + r.height };
	Vec2 p4 = { r.left + r.width, r.top };

	int b1 = grnd.GetBlock(p1);
	int b2 = grnd.GetBlock(p2);
	int b3 = grnd.GetBlock(p3);
	int b4 = grnd.GetBlock(p4);

	if (CollidesWith(b1)) {
		if (b1 == 0 || (b1 == 1 && canBreakSteel))
			grnd.setBlock(p1, -1);
		collided = true;
	}
	if (CollidesWith(b2)) {
		if (b2 == 0 || (b2 == 1 && canBreakSteel))
			grnd.setBlock(p2, -1);
		collided = true;
	}
	if (CollidesWith(b3)) {
		if (b3 == 0 || (b3 == 1 && canBreakSteel))
			grnd.setBlock(p3, -1);
		collided = true;
	}
	if (CollidesWith(b4)) {
		if (b4 == 0 || (b4 == 1 && canBreakSteel))
			grnd.setBlock(p4, -1);
		collided = true;
	}
}

bool Bullet::Collided() const
{
	return collided;
}