#include "TileMap.h"

#include "Ground.h"
#include <fstream>

int TileMap::GetBlock(const Vec2& pos) const
{
	Vei2 gp = toMapPos(pos);
	if (gp.x < 0 || gp.x > dim.x - 1 || gp.y < 0 || gp.y > dim.y - 1)
		return -2; // using -2 as 'out of the map' (-1 is the empty one)
	return mMap[gp.y * dim.x + gp.x];
}

Vei2 TileMap::toMapPos(const Vec2& p) const
{
	return { (int)floor(p.x / tileSize),
		(int)floor(p.y / tileSize) };
}

void TileMap::setBlock(const Vec2& pos, int block)
{
	Vei2 gp = toMapPos(pos);
	if (gp.x >= 0 && gp.x < dim.x &&
		gp.y >= 0 && gp.y < dim.y)
	{
		setBlock(gp.x, gp.y, block);
	}
}

void TileMap::setBlock(int x, int y, int block)
{
	mMap[y * dim.x + x] = block;

	sf::Vertex* quad = &m_vertices[(x + y * dim.x) * 4];

	if (block != -1) {
		// find its position in the tileset texture
		int tu = block % tilesPerRow;
		int tv = block / tilesPerRow;

		// define its 4 texture coordinates
		quad[0].texCoords = Vec2(texOring.x + tu * texRes, texOring.y + tv * texRes);
		quad[1].texCoords = Vec2(texOring.x + (tu + 1) * texRes, texOring.y + tv * texRes);
		quad[2].texCoords = Vec2(texOring.x + (tu + 1) * texRes, texOring.y + (tv + 1) * texRes);
		quad[3].texCoords = Vec2(texOring.x + tu * texRes, texOring.y + (tv + 1) * texRes);
	}
	else {
		quad[0].texCoords =
			quad[1].texCoords =
			quad[2].texCoords =
			quad[3].texCoords = Vec2(0.f, 0.f);
	}
}

TileMap::TileMap(Vei2 dimension, int tileSize, Texture& tex, int texRes,
	Vei2 texOring, int tilesPerRow)
	:
	dim(dimension),
	tileSize(tileSize),
	m_tex(tex),
	texRes(texRes),
	texOring(texOring),
	tilesPerRow(tilesPerRow)
{
	mMap = new int[dim.x * dim.y];
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(dim.x * dim.y * 4);

	for (unsigned int i = 0; i < dim.x; ++i)
		for (unsigned int j = 0; j < dim.y; ++j)
		{
			// initializes with empty
			mMap[j * dim.x + i] = -1;

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &m_vertices[(i + j * dim.x) * 4];

			// define its 4 corners
			quad[0].position = Vec2(i * tileSize, j * tileSize);
			quad[1].position = Vec2((i + 1) * tileSize, j * tileSize);
			quad[2].position = Vec2((i + 1) * tileSize, (j + 1) * tileSize);
			quad[3].position = Vec2(i * tileSize, (j + 1) * tileSize);
		}
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();
	// apply the tileset texture
	states.texture = &m_tex;

	// draw the vertex array
	target.draw(m_vertices, states);
}

int TileMap::getTileSize() const
{
	return tileSize;
}

Vei2 TileMap::getDim() const
{
	return dim;
}

TileMap::~TileMap()
{
	delete[] mMap;
}
