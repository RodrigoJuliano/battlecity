#include "Entity.h"
#include <SFML/Graphics/Rect.hpp>

Entity::Entity(Texture& tex, IntRect firstframe, int nFrames,
	float collisionsize, float frameTime)
	:
	cSprite(tex, firstframe, nFrames, collisionsize, frameTime)
{
}

Vec2 Entity::GetVel() const
{
	return velocity;
}

void Entity::setVel(Vec2 vel)
{
	velocity = vel;

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

bool Entity::CollidesWith(int block) const
{
	return (block != -1 && block != 2);
}

void Entity::update(float dt, Ground& grnd)
{
	Animable::update(dt);
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