#pragma once
#include <SFML/Graphics.hpp>
#include "Graphics.h"
#include "Animable.h"
#include "Ground.h"

using namespace sf;
using namespace std;

class Entity : public Drawable, public Transformable, public Animable
{
protected:
	Vec2 velocity;
	IntRect fframe;
	int id;
private:
	Texture& texture;
	VertexArray shape = VertexArray(Quads, 4);
public:
	Entity(Texture& tex, IntRect firstframe, int nFrames);
	Vec2 GetVel() const;
	void setVel(Vec2 vel);
	Vec2 GetDirection() const;
	FloatRect getGlobalBounds() const;
	Texture& getTexture() const;
	virtual bool CollidesWith(int block) const;
	virtual void update(float dt, Ground& grnd);
	virtual FloatRect getCollisionBox() const;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void onFrameChanged();
};