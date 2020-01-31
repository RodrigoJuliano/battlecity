#pragma once

#include "Animable.h"
#include "Gfx.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class cSprite : public Drawable, public Transformable, public Animable
{
private:
	Texture& texture;
	VertexArray shape = VertexArray(Quads, 4);
	IntRect fframe;
	float collisionSize;
	float scale;
public:
	cSprite(Texture& tex, IntRect firstframe, int nFrames, float collSize,
		float frameTime = 0.1f, float scale = Gfx::TextureScaleMult);
	Texture& getTexture() const;
	void constructShape();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void onFrameChanged();
	virtual FloatRect getCollisionBox() const;
	void setTextureRect(IntRect rect);
};

