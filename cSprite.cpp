#include "cSprite.h"
#include "Vec2.h"

cSprite::cSprite(Texture& tex, IntRect firstframe, int nFrames,
	float collSize, float frameTime, float scale)
	:
	Animable(nFrames, frameTime),
	texture(tex),
	fframe(firstframe),
	collisionSize(collSize),
	scale(scale)
{
	constructShape();
}

void cSprite::constructShape()
{
	shape[0].position = {0.f, 0.f};
	shape[1].position = {float(fframe.width), 0.f};
	shape[2].position = {float(fframe.width), float(fframe.height)};
	shape[3].position = {0.f, float(fframe.height)};

	shape[0].texCoords = {float(fframe.left), float(fframe.top)};
	shape[1].texCoords = {float(fframe.left + fframe.width), float(fframe.top)};
	shape[2].texCoords = {float(fframe.left + fframe.width), float(fframe.top + fframe.height)};
	shape[3].texCoords = {float(fframe.left), float(fframe.top + fframe.height)};

	setOrigin({ fframe.width * 0.5f, fframe.height * 0.5f });
	setScale(scale, scale);
}

Texture& cSprite::getTexture() const
{
	return texture;
}

void cSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &texture;
	target.draw(shape, states);
}

void cSprite::onFrameChanged()
{
	shape[0].texCoords = {float(fframe.left + fframe.width * curFrame), float(fframe.top)};
	shape[1].texCoords = {float(fframe.left + fframe.width * (curFrame + 1)), float(fframe.top)};
	shape[2].texCoords = {float(fframe.left + fframe.width * (curFrame + 1)), float(fframe.top + fframe.height)};
	shape[3].texCoords = {float(fframe.left + fframe.width * curFrame), float(fframe.top + fframe.height)};
}

FloatRect cSprite::getCollisionBox() const
{
	return FloatRect(getPosition() - Vec2(collisionSize / 2, collisionSize / 2),
		Vec2(collisionSize, collisionSize));
}

void cSprite::setTextureRect(IntRect rect)
{
	fframe = rect;
	constructShape();
}
