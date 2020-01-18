#pragma once

#include <SFML/Graphics.hpp>
#include "Graphics.h"
#include "Animable.h"

using namespace sf;

class Ground : public Drawable, public Transformable, public Animable
{
public:
	static const int cols = 26;
	static const int rows = 26;
	int blockSize = int(Gfx::TextureResolution * Gfx::TextureScaleMult);
private:
	int mGround[cols][rows];
	sf::VertexArray m_vertices;
	sf::Texture& m_tex;
public:
	Ground(Texture& tex);
	void setBlock(const Vec2& pos, int);
	int GetBlock(const Vec2& pos) const;
	Vei2 toGroundPos(const Vec2& p) const;
	void update(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void onFrameChanged();
};