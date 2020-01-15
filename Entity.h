#pragma once
#include <SFML/Graphics.hpp>
#include "Graphics.h"
#include "Block.h"

using namespace sf;
using namespace std;

class Entity : public Sprite
{
protected:
	Vec2 velocity;
	IntRect firstframe;
private:
	int nFrames;
	const float spf = 0.1f; // secs per frame
	float curframetime = 0.0f;
public:
	Entity(int id, Texture& tex, IntRect firstframe, int nFrames, Vec2 pos);
	Vec2 GetVel() const;
	void setVel(Vec2 vel);
	virtual bool CollidesWith(const Block* b) const;

	virtual void update(float dt);
	virtual FloatRect getCollisionBox() const;
	Vec2 GetDirection() const;
};