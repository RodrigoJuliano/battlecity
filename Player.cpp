#include "Player.h"

Player::Player(Texture& tex, IntRect firstframe, IntRect shieldfframe)
	:
	Tank(tex, firstframe, 1, 330.f),
	shield(tex, shieldfframe, 2, 0.f, 0.01f)
{
}

void Player::addShield(float time)
{
	shielded = true;
	shieldTime = time;
	shield.setPosition(getPosition());
}

bool Player::isShielded()
{
	return shielded;
}

void Player::addLife()
{
	lifes++;
}

int Player::getNumLifes()
{
	return lifes;
}

void Player::decNumLifes()
{
	lifes--;
}

void Player::setNumLifes(int nLifes)
{
	lifes = nLifes;
}

void Player::addStar()
{
	stars++;
	switch (stars)
	{
	case 1:
		setTextureRect({0, 16, 13, 16});
		setBulletSpeed(500.f);
		break;
	case 2:
		setTextureRect({ 0, 32, 13, 15 });
		setMaxFire(2);
		break;
	case 3:
		setTextureRect({ 0, 48, 14, 15 });
		break;
	}
}

int Player::getNumStars()
{
	return stars;
}

void Player::resetStars()
{
	stars = 0;
	setTextureRect({ 0, 0, 13, 13 });
	setBulletSpeed(330.f);
	setMaxFire(1);
}

bool Player::isSpawning() const
{
	return spawning;
}

void Player::setSpawning(bool spawning)
{
	this->spawning = spawning;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Entity::draw(target, states);
	if (shielded)
		target.draw(shield, states);
}

void Player::update(float dt, Ground& grnd)
{
	Tank::update(dt, grnd);
	if (shielded) {
		shieldTime -= dt;
		if (shieldTime < 0.f)
			shielded = false;
		else {
			shield.update(dt);
			shield.setPosition(getPosition());
		}
	}
}
