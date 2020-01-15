#pragma once

#include <SFML/Graphics.hpp>
#include "Block.h"
#include "Graphics.h"

class Ground
{
public:
	static const int cols = 26;
	static const int rows = 26;
	int blockSize = int(Gfx::TextureResolution * Gfx::TextureScaleMult);
private:
	Block* mGround[cols][rows];
	// edges
	RectangleShape edges;
public:
	Ground();
	void Draw(RenderWindow& rw);
	void setBlock(const Vec2& pos, Block* block);
	Block* GetBlock(const Vec2& pos) const;
	Vec2 toScreenPos(const Vei2& p) const;
	Vei2 toGroundPos(const Vec2& p) const;
	FloatRect getBlockBoundsAt(const Vec2& p) const;
	void update(float dt);
};