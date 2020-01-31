#pragma once

#include "Gfx.h"
#include "Vec2.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class TileMap : public Drawable, public Transformable
{
protected:
	Vei2 dim;
	int tileSize;
	int tilesPerRow;
	int* mMap;
	sf::VertexArray m_vertices;
	sf::Texture& m_tex;
	int texRes;
	Vei2 texOring;
public:
	void setBlock(int x, int y, int tile);
	void setBlock(const Vec2& pos, int tile); // screen relative pos 
	TileMap(Vei2 dimension, int tileSize, Texture& tex, int texRes,
		Vei2 texOring, int tilesPerRow);
	int GetBlock(const Vec2& pos) const;
	Vei2 toMapPos(const Vec2& p) const;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	int getTileSize() const;
	Vei2 getDim() const;
	~TileMap();
};