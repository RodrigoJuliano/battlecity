#pragma once

#include "Ground.h"
#include "Entity.h"

class Bullet : public Entity {
private:
	bool collided = true;
public:
	Bullet(Texture& tex, IntRect texRect);
	bool CollidesWith(int block) const;
	void update(float dt, Ground& grnd);
	bool Collided() const;
	void spawn(Vec2 pos);
	FloatRect getCollisionBox() const;
	virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const {};
};