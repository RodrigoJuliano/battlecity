#include "cSprite.h"
#include "Graphics.h"

cSprite::cSprite(Texture& tex, IntRect firstframe, int nFrames, float collSize, float frameTime)
	:
	Animable(nFrames, frameTime),
	texture(tex),
	fframe(firstframe),
	collisionSize(collSize)
{
	constructShape();
}

void cSprite::constructShape()
{
	shape[0].position = Vec2(0.f, 0.f);
	shape[1].position = Vec2(fframe.width, 0.f);
	shape[2].position = Vec2(fframe.width, fframe.height);
	shape[3].position = Vec2(0.f, fframe.height);

	shape[0].texCoords = Vec2(fframe.left, fframe.top);
	shape[1].texCoords = Vec2(fframe.left + fframe.width, fframe.top);
	shape[2].texCoords = Vec2(fframe.left + fframe.width, fframe.top + fframe.height);
	shape[3].texCoords = Vec2(fframe.left, fframe.top + fframe.height);

	setOrigin({ fframe.width * 0.5f, fframe.height * 0.5f });
	setScale(Gfx::TextureScaleMult, Gfx::TextureScaleMult);
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
	shape[0].texCoords = Vec2(fframe.left + fframe.width * curFrame, fframe.top);
	shape[1].texCoords = Vec2(fframe.left + fframe.width * (curFrame + 1), fframe.top);
	shape[2].texCoords = Vec2(fframe.left + fframe.width * (curFrame + 1), fframe.top + fframe.height);
	shape[3].texCoords = Vec2(fframe.left + fframe.width * curFrame, fframe.top + fframe.height);
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
