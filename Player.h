#pragma once

#include "Tank.h"

class Player : public Tank
{
private:
	int lifes = 2;
	bool shielded = false;
	float shieldTime = 0.f;
	int stars = 0;
	cSprite shield;
public:
	Player(Texture& tex, IntRect firstframe, IntRect shieldfframe);
	void addShield(float time);
	bool isShielded();
	void addLife();
	int getNumLifes();
	void decNumLifes();
	void setNumLifes(int nLifes);
	void addStar();
	int getNumStars();
	void resetStars();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void update(float dt, Ground& grnd);
};

