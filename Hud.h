#pragma once

#include <SFML/Graphics.hpp>
#include "cSprite.h"
#include "TileMap.h"
#include "Counter.h"

using namespace sf;

class Hud : public Drawable
{
private:
	Screen& curScreen;
	Texture& tex;
	RectangleShape edges1;
	RectangleShape hudback;
	// Play screen
	cSprite lifesholder;
	cSprite levelholder;
	Counter lifes;
	Counter level;
	TileMap enemCounter;
	int nEnemies;
	// Start screen
	Texture titleTex;
	cSprite title;
	cSprite selecter;
	int selected = 0;
	// Select stage screen
	cSprite stage;
	Counter stageNumText;
	int stageNum = 1;
	const int numStages;
	// Pause screen
	cSprite pause;
	// Game over screen
	cSprite gameOver;
	// Next stage screen
	const float screenTime = 3.f;
	float curScreenTime = 0.f;
	bool endScreenTime = false;
public:
	Hud(Texture& tex, Screen& curScreen, int numStages);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	// Play screen
	void setLifes(int lifes);
	void removeEnemy();
	void resetEnemCounter();
	// Start screen
	void toggleSelect();
	int getSelected() const;
	// Select stage screen
	void prevStage();
	void nextStage();
	int getSelStage() const;
	// Pause screen
	void update(float dt);
	// Next stage screen
	bool canGoNextStage();
	void resetTime();
};

