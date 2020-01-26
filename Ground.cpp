#include "Ground.h"
#include <fstream>

Ground::Ground(Vei2 dimension, int tileSize, Texture& tex, int texRes,
	Vei2 texOring, int tilesPerRow, int nFrames, float frameTime)
	:
	TileMap(dimension, tileSize, tex, texRes, texOring, tilesPerRow),
	Animable(nFrames, frameTime),
	trees(dimension, tileSize, tex, texRes, texOring, tilesPerRow)
{
}

void Ground::onFrameChanged()
{
	for (unsigned int i = 0; i < dim.x; ++i)
		for (unsigned int j = 0; j < dim.y; ++j) {

			int id;
			if ((id = mMap[j * dim.y + i]) == 4) { // water tile

				// get a pointer to the current tile's quad
				sf::Vertex* quad = &m_vertices[(i + j * dim.x) * 4];

				// find its position in the tileset texture
				int tu = (id + curFrame) % tilesPerRow;
				int tv = id / tilesPerRow;

				// define its 4 texture coordinates
				quad[0].texCoords = Vec2(texOring.x + tu * texRes, texOring.y + tv * texRes);
				quad[1].texCoords = Vec2(texOring.x + (tu + 1) * texRes, texOring.y + tv * texRes);
				quad[2].texCoords = Vec2(texOring.x + (tu + 1) * texRes, texOring.y + (tv + 1) * texRes);
				quad[3].texCoords = Vec2(texOring.x + tu * texRes, texOring.y + (tv + 1) * texRes);
			}
		}
}

bool Ground::saveToFile(std::string file)
{
	std::ofstream f(file);
	if (f.fail()) {
		f.close();
		return false;
	}
	// default num of tanks
	f << 8 << " ";
	f << 5 << " ";
	f << 4 << " ";
	f << 3 << " \n";

	for (unsigned int i = 0; i < dim.x; ++i) {
		for (unsigned int j = 0; j < dim.y; ++j) {
			f << mMap[j * dim.x + i] << " ";
		}
		f << "\n";
	}
	f.close();
	return true;
}

bool Ground::loadFromStream(std::istream& stream)
{
	int b;
	for (unsigned int i = 0; i < dim.x; ++i) {
		for (unsigned int j = 0; j < dim.y; ++j) {
			stream >> b;
			setBlock(i, j, b);
		}
	}
	return true;
}

void Ground::setBlock(int x, int y, int tile)
{
	TileMap::setBlock(x, y, tile);
	if (tile == 2)
		trees.setBlock(x, y, tile);
	else
		trees.setBlock(x, y, -1);
}

void Ground::setBlock(const Vec2& pos, int tile)
{
	TileMap::setBlock(pos, tile);
	if (tile == 2)
		trees.setBlock(pos, tile);
	else
		trees.setBlock(pos, -1);
}

void Ground::drawTrees(ScreenArea& sa)
{
	sa.draw(trees);
}
