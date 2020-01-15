#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class Block : public Sprite {
private:
	int id;
	int nFrames;
	IntRect firstframe;
	const float spf = 0.3f; // secs per frame
	float curframetime = 0.0f;
public:
	Block(int id, Texture& tex, IntRect texRect, int nFrames = 1);
	int getId() const;
	static constexpr int nBlocks = 20;
	static Block* mBlocks[nBlocks];
	void update(float dt);
};