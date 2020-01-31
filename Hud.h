#pragma once

#include "cSprite.h"
#include "TileMap.h"
#include "Counter.h"
#include "Screen.h"
#include <SFML/Graphics.hpp>

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
	// Custruct screen
	TileMap tilePicker;
	cSprite tileSel;
	const int startSelec = 12;
	int selectedTile;
public:
	Hud(Texture& tex, Screen& curScreen, int numStages);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float dt);
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
	// Game over screen
	void resetGameOverPos();
	// Next stage screen
	bool canGoNextStage();
	void resetTime();
	// Custruct screen
	int pickTile(Vec2 pos);
	Vec2 getPickerPos();
	void setSelecTile(int tile);
	int getSelecTile() const;
};

