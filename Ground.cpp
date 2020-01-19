#include "Ground.h"

int Ground::GetBlock(const Vec2& pos) const
{
	Vei2 gp = toGroundPos(pos);
	// Trick to entities stay inside the map
	if (gp.x < 0 || gp.x > cols-1 || gp.y < 0 || gp.y > rows-1)
		return 0;
	return mGround[gp.x][gp.y];
}

Vei2 Ground::toGroundPos(const Vec2& p) const
{
	return {(int)floor(p.x / blockSize),
		(int)floor(p.y / blockSize)};
}

void Ground::setBlock(const Vec2& pos, int block)
{
	Vei2 gp = toGroundPos(pos);
	if (gp.x >= 0 && gp.x < cols &&
		gp.y >= 0 && gp.y < rows)
	{
		setBlock(gp.x, gp.y, block);
	}
}

void Ground::setBlock(int x, int y, int block)
{
	mGround[x][y] = block;

	sf::Vertex* quad = &m_vertices[(x + y * cols) * 4];

	if (block != -1) {
		// find its position in the tileset texture
		int tu = block % 4; // 4 tiles per row
		int tv = block / 4;

		// define its 4 texture coordinates
		quad[0].texCoords = Vec2(64 + tu * Gfx::TextureResolution, tv * Gfx::TextureResolution);
		quad[1].texCoords = Vec2(64 + (tu + 1) * Gfx::TextureResolution, tv * Gfx::TextureResolution);
		quad[2].texCoords = Vec2(64 + (tu + 1) * Gfx::TextureResolution, (tv + 1) * Gfx::TextureResolution);
		quad[3].texCoords = Vec2(64 + tu * Gfx::TextureResolution, (tv + 1) * Gfx::TextureResolution);
	}
	else {
		quad[0].texCoords =
			quad[1].texCoords =
			quad[2].texCoords =
			quad[3].texCoords = Vec2(0.f, 0.f);
	}
}

Ground::Ground(Texture& tex)
	:
	Animable(3, 0.3f),
	m_tex(tex)
{
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(cols * rows * 4);

	for (unsigned int i = 0; i < cols; ++i)
		for (unsigned int j = 0; j < rows; ++j)
		{
			// initializes with empty
			mGround[i][j] = -1;

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &m_vertices[(i + j * cols) * 4];

			// define its 4 corners
			quad[0].position = Vec2(i * blockSize, j * blockSize);
			quad[1].position = Vec2((i + 1) * blockSize, j * blockSize);
			quad[2].position = Vec2((i + 1) * blockSize, (j + 1) * blockSize);
			quad[3].position = Vec2(i * blockSize, (j + 1) * blockSize);
		}
}

void Ground::update(float dt)
{
	Animable::update(dt);
}

void Ground::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();
	// apply the tileset texture
	states.texture = &m_tex;

	// draw the vertex array
	target.draw(m_vertices, states);
}

void Ground::onFrameChanged()
{
	for (unsigned int i = 0; i < cols; ++i)
		for (unsigned int j = 0; j < rows; ++j) {

			int id;
			if (mGround[i][j] != -1 &&
				(id = mGround[i][j]) > 11) {

				// get a pointer to the current tile's quad
				sf::Vertex* quad = &m_vertices[(i + j * cols) * 4];

				// find its position in the tileset texture
				int tu = (id + curFrame) % 4; // 4 tiles per row
				int tv = id / 4;

				// define its 4 texture coordinates
				quad[0].texCoords = Vec2(64 + tu * Gfx::TextureResolution, tv * Gfx::TextureResolution);
				quad[1].texCoords = Vec2(64 + (tu + 1) * Gfx::TextureResolution, tv * Gfx::TextureResolution);
				quad[2].texCoords = Vec2(64 + (tu + 1) * Gfx::TextureResolution, (tv + 1) * Gfx::TextureResolution);
				quad[3].texCoords = Vec2(64 + tu * Gfx::TextureResolution, (tv + 1) * Gfx::TextureResolution);
			}
		}
}
