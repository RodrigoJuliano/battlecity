#include "Entity.h"
#include <SFML/Graphics/Rect.hpp>

Entity::Entity(int id, Texture& tex, IntRect firstframe, int nFrames)
	:
	id(id),
	texture(tex),
	fframe(firstframe),
	nFrames(nFrames)
{
	shape[0].position = Vec2( 0.f, 0.f );
	shape[1].position = Vec2( fframe.width, 0.f );
	shape[2].position = Vec2( fframe.width, fframe.height );
	shape[3].position = Vec2( 0.f, fframe.height );

	shape[0].texCoords = Vec2(fframe.left , fframe.top);
	shape[1].texCoords = Vec2(fframe.left + fframe.width , fframe.top);
	shape[2].texCoords = Vec2(fframe.left + fframe.width, fframe.top + fframe.height);
	shape[3].texCoords = Vec2(fframe.left , fframe.top + fframe.height);

	setOrigin({ fframe.width * 0.5f, fframe.height * 0.5f });
	setScale(Gfx::TextureScaleMult, Gfx::TextureScaleMult);
}

Vec2 Entity::GetVel() const
{
	return velocity;
}

void Entity::setVel(Vec2 vel)
{
	velocity = vel;
}

bool Entity::CollidesWith(int block) const
{
	return (block != -1 && block != 2);
}

void Entity::update(float dt)
{
	if(nFrames > 1)
		// only animate if are moving
		if (abs(velocity.x) > 0.f || abs(velocity.y) > 0.f) {
			if (curframetime > spf) {
				curframetime = 0.0f;
				// change the frame
				curFrame++;
				if (curFrame > nFrames - 1)
					curFrame = 0;

				shape[0].texCoords = Vec2( fframe.left + fframe.width * curFrame, fframe.top );
				shape[1].texCoords = Vec2( fframe.left + fframe.width * (curFrame + 1), fframe.top );
				shape[2].texCoords = Vec2(fframe.left + fframe.width * (curFrame + 1), fframe.top + fframe.height);
				shape[3].texCoords = Vec2( fframe.left + fframe.width * curFrame, fframe.top + fframe.height );
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
	r.top += 3;
	r.left += 3;
	r.height -= 6;
	r.width -= 6;
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

FloatRect Entity::getGlobalBounds() const
{
	FloatRect r(shape[0].position, Vec2( fframe.width, fframe.height ));
	
	auto t = getTransform();

	auto tr = t.transformRect(r);

	return tr;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	onDraw(target, states);
	states.transform *= getTransform();
	states.texture = &texture;
	target.draw(shape, states);
}

