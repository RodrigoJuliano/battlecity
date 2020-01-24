#pragma once

#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include "Animable.h"

using namespace sf;

class Ground : public TileMap, public Animable
{
public:
	Ground(Vei2 dimension, int tileSize, Texture& tex, int texRes,
		Vei2 texOring, int tilesPerRow, int nFrames, float frameTime);
	virtual void onFrameChanged();
	bool saveToFile(std::string file);
	bool loadFromStream(std::istream& stream);
};