#pragma once

#include <SFML/Graphics.hpp>
#include "Graphics.h"

using namespace sf;

class Ground : public Drawable, public Transformable
{
public:
	static const int cols = 26;
	static const int rows = 26;
	int blockSize = int(Gfx::TextureResolution * Gfx::TextureScaleMult);
private:
	int mGround[cols][rows];
	sf::VertexArray m_vertices;
	sf::Texture& m_tex;
	int curFrame = 0;
	int nFrames = 3;
	const float spf = 0.3f; // secs per frame
	float curframetime = 0.0f;
public:
	Ground(Texture& tex);
	void setBlock(const Vec2& pos, int);
	int GetBlock(const Vec2& pos) const;
	Vei2 toGroundPos(const Vec2& p) const;
	void update(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};