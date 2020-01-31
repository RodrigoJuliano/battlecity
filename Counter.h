#pragma once

#include "TileMap.h"

class Counter : public TileMap
{
public:
	Counter(int nChars, Texture& tex);
	void setNumber(int num);
	void setPosition(Vec2 pos);
};

