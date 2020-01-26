#pragma once

#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include "Animable.h"
#include "ScreenArea.h"

using namespace sf;

class Ground : public TileMap, public Animable
{
private:
	TileMap trees;
public:
	Ground(Vei2 dimension, int tileSize, Texture& tex, int texRes,
		Vei2 texOring, int tilesPerRow, int nFrames, float frameTime);
	virtual void onFrameChanged();
	bool saveToFile(std::string file);
	bool loadFromStream(std::istream& stream);
	void setBlock(int x, int y, int tile);
	void setBlock(const Vec2& pos, int tile);
	void drawTrees(ScreenArea& sa);
};