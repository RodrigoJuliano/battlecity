#include "Counter.h"

Counter::Counter(int nChars, Texture& tex)
	:
	TileMap({ nChars, 1 }, Gfx::TileSize, tex, Gfx::TextureResolution, { 136 , 184 }, 5)
{

}

void Counter::setNumber(int num) {
	int div = num / 10;
	int res = num % 10;
	int pos = getDim().x - 1;
	do {
		setBlock(pos, 0, res);
		res = div % 10;
		div /= 10;
		pos--;
	} while (pos > -1);
}

void Counter::setPosition(Vec2 pos) {
	TileMap::setPosition(pos);
}