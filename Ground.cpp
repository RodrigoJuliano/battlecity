#include "Ground.h"
#include <fstream>

Ground::Ground(Vei2 dimension, int tileSize, Texture& tex, int texRes,
	Vei2 texOring, int tilesPerRow, int nFrames, float frameTime)
	:
	TileMap(dimension, tileSize, tex, texRes, texOring, tilesPerRow),
	Animable(nFrames, frameTime)
{
}

void Ground::onFrameChanged()
{
	for (unsigned int i = 0; i < dim.x; ++i)
		for (unsigned int j = 0; j < dim.y; ++j) {

			int id;
			if (mMap[j * dim.x + i] != -1 &&
				(id = mMap[j * dim.y + i]) > 11) {

				// get a pointer to the current tile's quad
				sf::Vertex* quad = &m_vertices[(i + j * dim.x) * 4];

				// find its position in the tileset texture
				int tu = (id + curFrame) % tilesPerRow; // 4 tiles per row
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

	for (unsigned int i = 0; i < dim.x; ++i) {
		for (unsigned int j = 0; j < dim.y; ++j) {
			f << mMap[j * dim.x + i] << " ";
		}
		f << "\n";
	}
	f.close();
	return true;
}

bool Ground::loadFromFile(std::string file)
{
	std::ifstream f(file);
	if (f.fail()) {
		f.close();
		return false;
	}
	int b;
	for (unsigned int i = 0; i < dim.x; ++i) {
		for (unsigned int j = 0; j < dim.y; ++j) {
			f >> b;
			setBlock(i, j, b);
		}
	}
	f.close();
	return true;
}