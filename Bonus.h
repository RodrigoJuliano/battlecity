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
	Type type;
	float time = 30.f;
public:
	Bonus(Texture& tex, IntRect firstframe, Type type)
		:
		cSprite(tex, firstframe, 2, 30.f, 0.3f),
		type(type) {

	}

	virtual void update(float dt) {
		cSprite::update(dt);
		time -= dt;
		frameTime = time / 100.f; // blinks faster over time
	}

	float getTime() {
		return time;
	}

	Type getType() {
		return type;
	}
};

