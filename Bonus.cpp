#include "Bonus.h"

Bonus::Bonus(Texture& tex, IntRect firstframe, Type type)
	:
	cSprite(tex, firstframe, 2, 30.f, 0.3f),
	type(type) {

}

void Bonus::update(float dt) {
	cSprite::update(dt);
	time -= dt;
	frameTime = time / 100.f; // blinks faster over time
}

float Bonus::getTime() {
	return time;
}

Bonus::Type Bonus::getType() {
	return type;
}
