#include "Tank.h"

Tank::Tank(int id, Texture& tex, IntRect firstframe)
	:
	Entity(tex, firstframe, 2)
{
}

void Tank::update(float dt, Ground& grnd)
{
	// only update direction and animation if are moving
	if (abs(velocity.x) > 0.f || abs(velocity.y) > 0.f)
		Entity::update(dt, grnd);

	// Do the entity move checking for collision with map

	// Update the x coord
	Vec2 p = getPosition();
	p.x += velocity.x * dt;
	setPosition(p);

	// If collided move back to the side of the block

	// Left
	auto r = getCollisionBox();
	int b1 = grnd.GetBlock({ r.left, r.top });
	int b2 = grnd.GetBlock({ r.left, r.top + r.height / 2 });
	int b3 = grnd.GetBlock({ r.left, r.top + r.height });

	if (CollidesWith(b1) || CollidesWith(b2) || CollidesWith(b3)) {
		// set the x position back to the empty block on the right
		// basicaly get the relative ground position, sum 1 and convert back to screen position
		setPosition({ int(floor(r.left / grnd.blockSize) + 1) * grnd.blockSize + ceil(r.width / 2), p.y });
	}
	// right
	r = getCollisionBox();
	b1 = grnd.GetBlock({ r.left + r.width, r.top });
	b2 = grnd.GetBlock({ r.left + r.width, r.top + r.height/2 });
	b3 = grnd.GetBlock({ r.left + r.width, r.top + r.height });
	if (CollidesWith(b1) || CollidesWith(b2) || CollidesWith(b3)) {
		// set the x position back to the empty block on the left
		setPosition({ int((r.left + r.width)/ grnd.blockSize) * grnd.blockSize - ceil(r.width / 2), p.y });
	}

	// Update the y coord
	p = getPosition();
	p.y += velocity.y * dt;
	setPosition(p);

	// If collided move back to the side of the block

	// up
	r = getCollisionBox();
	b1 = grnd.GetBlock({ r.left, r.top });
	b2 = grnd.GetBlock({ r.left + r.width /2, r.top });
	b3 = grnd.GetBlock({ r.left + r.width, r.top });

	if (CollidesWith(b1) || CollidesWith(b2) || CollidesWith(b3)) {
		// set the x position back to the empty block below
		setPosition({ p.x, int(floor(r.top / grnd.blockSize) + 1) * grnd.blockSize + ceil(r.height / 2) });
	}
	// down
	r = getCollisionBox();
	b1 = grnd.GetBlock({ r.left, r.top + r.height });
	b2 = grnd.GetBlock({ r.left + r.width, r.top + r.height });
	b3 = grnd.GetBlock({ r.left + r.width / 2, r.top + r.height });
	if (CollidesWith(b1) || CollidesWith(b2) || CollidesWith(b3)) {
		// set the x position back to the empty block above
		setPosition({ p.x, int((r.top + r.height ) / grnd.blockSize) * grnd.blockSize - ceil(r.height / 2) });
	}
}