#include "Entity.h"
#include <SFML/Graphics/Rect.hpp>

Entity::Entity(int id, Texture& tex, IntRect firstframe, int nFrames, Vec2 pos)
	:
	Sprite(tex, firstframe),
	firstframe(firstframe),
	nFrames(nFrames)
{
	setPosition(pos);
}

Vec2 Entity::GetVel() const
{
	return velocity;
}

void Entity::setVel(Vec2 vel)
{
	velocity = vel;
}

bool Entity::CollidesWith(const Block* b) const
{
	return (b != nullptr && b->getId() != 2);
}

void Entity::update(float dt)
{
	if(nFrames > 1)
		// only animate if are moving
		if (abs(velocity.x) > 0.f || abs(velocity.y) > 0.f) {
			if (curframetime > spf) {
				curframetime = 0.0f;
				// change the frame
				IntRect r = getTextureRect();
				if (r.left == firstframe.left + (r.width+1) * (nFrames-1))
					setTextureRect(firstframe);
				else
					setTextureRect({ r.left + r.width+1, r.top, r.width, r.height });
			}
			else {
				curframetime += dt;
			}
		}

	// Rotate the sprite

	if (velocity.x < 0.0f) {
		setRotation(-90.0f); // Left
	}
	else if (velocity.x > 0.0f) {
		setRotation(90.0f); // Right
	}
	else if (velocity.y < 0.0f) {
		setRotation(0.0f); // Up
	}
	else if (velocity.y > 0.0f) {
		setRotation(180.0f); // Down
	}
}

FloatRect Entity::getCollisionBox() const
{
	auto r = getGlobalBounds();
	// reduce the box a little bit
	r.top += 2;
	r.left += 2;
	r.height -= 3;
	r.width -= 3;
	return r;
}

Vec2 Entity::GetDirection() const
{
	float r = getRotation();
	if (r == 0.f)
		return { 0.f,-1.f };
	if (r == 90.f)
		return { 1.f,0.f };
	if(r == 180.f)
		return { 0.f,1.f };
	//if(r == 270.f)
		return { -1.f,0.f };
}

