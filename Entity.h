#pragma once
#include <SFML/Graphics.hpp>
#include "Graphics.h"
#include "Block.h"

using namespace sf;
using namespace std;

class Entity : public Drawable, public Transformable
{
protected:
	Vec2 velocity;
	IntRect fframe;
	int id;
private:
	Texture& texture;
	VertexArray shape = VertexArray(Quads, 4);
	int nFrames;
	int curFrame = 0;
	const float spf = 0.1f; // secs per frame
	float curframetime = 0.0f;
public:
	Entity(int id, Texture& tex, IntRect firstframe, int nFrames);
	Vec2 GetVel() const;
	void setVel(Vec2 vel);
	Vec2 GetDirection() const;
	FloatRect getGlobalBounds() const;
	virtual bool CollidesWith(int block) const;
	virtual void update(float dt);
	virtual FloatRect getCollisionBox() const;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};