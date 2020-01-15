#include "Ground.h"

Block* Ground::GetBlock(const Vec2& pos) const
{
	Vei2 gp = toGroundPos(pos);
	// Trick to entities stay inside the map
	if (gp.x < 0 || gp.x > cols-1 || gp.y < 0 || gp.y > rows-1)
		return Block::mBlocks[0];
	return mGround[gp.x][gp.y];
}

Vec2 Ground::toScreenPos(const Vei2& p) const
{
	return {float(p.x * blockSize) + Gfx::EdgeSize, float(p.y * blockSize) + Gfx::EdgeSize };
}

Vei2 Ground::toGroundPos(const Vec2& p) const
{
	return {(int)floor((p.x - Gfx::EdgeSize) / blockSize),
		(int)floor((p.y - Gfx::EdgeSize) / blockSize)};
}

void Ground::setBlock(const Vec2& pos, Block* block)
{
	Vei2 gp = toGroundPos(pos);
	if (gp.x >= 0 && gp.x < cols &&
		gp.y >= 0 && gp.y < rows)
	{
		mGround[gp.x][gp.y] = block;
	}
}

Ground::Ground()
{
	edges.setSize({ Gfx::ScreenWidth, Gfx::ScreenHeight });
	edges.setPosition(0.f, 0.f);
	edges.setFillColor(Color::Transparent);
	edges.setOutlineColor(Color(99, 99, 99));
	edges.setOutlineThickness(-Gfx::EdgeSize);
}

void Ground::Draw(RenderWindow& rw)
{
	// blocks
	for (int i = 0; i < cols; ++i)
	{
		for (int j = 0; j < rows; ++j)
		{
			Block* b = mGround[i][j];
			if (b != nullptr)
			{
				b->setPosition(toScreenPos({ i, j }));
				rw.draw(*b);
			}
		}
	}

	// edges
	rw.draw(edges);
}


FloatRect Ground::getBlockBoundsAt(const Vec2& p) const {
	Vei2 gp = toGroundPos(p);
	return { float(gp.x * blockSize), float(gp.y * blockSize), float(blockSize), float(blockSize) };
}

void Ground::update(float dt)
{
	Block::mBlocks[12]->update(dt);
}
