#pragma once

#include <SFML/Graphics.hpp>
#include "Animable.h"

using namespace sf;

class cSprite : public Drawable, public Transformable, public Animable
{
private:
	Texture& texture;
	VertexArray shape = VertexArray(Quads, 4);
	IntRect fframe;
	float collisionSize;
public:
	cSprite(Texture& tex, IntRect firstframe, int nFrames, float collSize, float frameTime = 0.1f);
	Texture& getTexture() const;
	void constructShape();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void onFrameChanged();
	virtual FloatRect getCollisionBox() const;
	void setTextureRect(IntRect rect);
};
