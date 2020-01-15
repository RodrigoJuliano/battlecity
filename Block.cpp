#include "Block.h"

Block* Block::mBlocks[Block::nBlocks];

Block::Block(int id, Texture& tex, IntRect texRect, int nFrames)
	:
	Sprite(tex, texRect),
	id(id),
	firstframe(texRect),
	nFrames(nFrames)
{
}

int Block::getId() const
{
	return id;
}

void Block::update(float dt)
{
	if (nFrames > 1)
		if (curframetime > spf) {
			curframetime = 0.0f;
			// change the frame
			IntRect r = getTextureRect();
			if (r.left == firstframe.left + r.width * (nFrames - 1))
				setTextureRect(firstframe);
			else
				setTextureRect({ r.left + r.width , r.top, r.width, r.height });
		}
		else {
			curframetime += dt;
		}
}
