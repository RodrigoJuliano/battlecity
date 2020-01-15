#include "Bullet.h"

Bullet::Bullet(Texture& tex, IntRect texRect, Vec2 pos) :
	Entity(10, tex, texRect, 1, pos)
{
}

bool Bullet::CollidesWith(const Block* b) const
{
	return Entity::CollidesWith(b) && b->getId() != 12;
}

void Bullet::update(float dt, Ground& grnd)
{
	Entity::update(dt);

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

	Block* b1 = grnd.GetBlock(p1);
	Block* b2 = grnd.GetBlock(p2);
	Block* b3 = grnd.GetBlock(p3);
	Block* b4 = grnd.GetBlock(p4);

	if (CollidesWith(b1)) {
		grnd.setBlock(p1, nullptr);
		collided = true;
	}
	if (CollidesWith(b2)) {
		grnd.setBlock(p2, nullptr);
		collided = true;
	}
	if (CollidesWith(b3)) {
		grnd.setBlock(p3, nullptr);
		collided = true;
	}
	if (CollidesWith(b4)) {
		grnd.setBlock(p4, nullptr);
		collided = true;
	}
}

bool Bullet::Collided()
{
	return collided;
}

FloatRect Bullet::getCollisionBox() const
{
	auto r = getGlobalBounds();
	// reduce the box a little bit
	r.top -= 2;
	r.left -= 2;
	r.height += 3;
	r.width += 3;
	return r;
}
