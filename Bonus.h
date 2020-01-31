#pragma once

#include "cSprite.h"

class Bonus : public cSprite
{
public:
	enum class Type {
		helmet = 0,
		timer,
		shovel,
		star,
		grenade,
		tank
	};
private:
	Type type;
	float time = 30.f;
public:
	Bonus(Texture& tex, IntRect firstframe, Type type);
	virtual void update(float dt);
	float getTime();
	Type getType();
};

