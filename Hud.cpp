#include "Hud.h"
#include "Graphics.h"

Hud::Hud(Texture& tex, Screen& curScreen, int numStages)
    :
    tex(tex),
    curScreen(curScreen),
    numStages(numStages),
    // Play screen
    lifesholder(tex, {184, 136, 16, 16}, 1, 0.f),
    levelholder(tex, {184, 184, 16, 15}, 1, 0.f),
    lifes(1, tex),
    level(2, tex),
    enemCounter({2, 10}, Gfx::TileSize, tex,
        Gfx::TextureResolution, {128, 192}, 1),
    // Start screen
    title(titleTex, {0, 0, 188, 111}, 1, 0.f),
    selecter(tex, { 0,0,13,13 }, 1, 0.f),
    // Select stage screen
    stage(tex, {136, 176, 39, 7}, 1, 0.f),
    stageNumText(2, tex),
    // Pause screen
    pause(tex, {97, 176, 38, 7}, 2, 0.f, 0.2f),
    gameOver(tex, {97, 184, 31, 15}, 1, 0.f),
    // Custruct screen
    tilePicker({2, 14}, Gfx::TileSize, tex,
        Gfx::TextureResolution, { 64, 0 }, 4),
    tileSel(tex, { 0, 128, 43, 7 }, 1, 0.f, 0.f, 1.5f)
{
    // screen edges
    edges1.setSize({ Gfx::ScreenWidth-40.f, Gfx::ScreenHeight });
    edges1.setFillColor(Color::Transparent);
    edges1.setOutlineColor(Color(99, 99, 99));
    edges1.setOutlineThickness(-Gfx::EdgeSize);
    hudback.setSize({ 60.f, Gfx::ScreenHeight });
    hudback.setFillColor(Color(99, 99, 99));
    // Play screen
    lifesholder.setPosition({ 20.f, 355.f });
    levelholder.setPosition({ 20.f, 480.f });
    lifes.setPosition({ 21.f, 358.f });
    level.setPosition({ 1.f, 507.f });
    level.setNumber(1);
    enemCounter.setPosition({0.f, 50.f});
    // Start screen
    titleTex.loadFromFile("resources\\StartScreen.png");
    selecter.setRotation(90.f);
    selecter.setPosition({200.f, 390.f});
    title.setPosition({Gfx::ScreenWidth/2, 300.f});
    // Select stage screen
    stage.setPosition({Gfx::ScreenWidth/2 - 30.f, Gfx::ScreenHeight/2});
    stageNumText.setPosition({Gfx::ScreenWidth/2 + 30.f, Gfx::ScreenHeight/2-9.f});
    stageNumText.setNumber(stageNum);
    // Pause screen
    pause.setPosition({Gfx::ScreenWidth/2, Gfx::ScreenHeight/2});
    // Game Over screen
    resetGameOverPos();
    // Custruct screen
    tilePicker.setPosition({ 0.f, 50.f });
    for (int i = 0; i < 5; i++) {
        int y = i * 2;
        tilePicker.setBlock(0, y, i);
        tilePicker.setBlock(0, y+1, i);
        tilePicker.setBlock(1, y, i);
        tilePicker.setBlock(1, y+1, i);
    }
    setSelecTile(0);
    tileSel.setPosition({20.f, 340.f});
}

void Hud::setLifes(int nlifes)
{
    lifes.setNumber(nlifes);
}

void Hud::removeEnemy()
{
    nEnemies--;
    int x = nEnemies % 2;
    int y = nEnemies / 2;
    enemCounter.setBlock(x, y, -1);
}

void Hud::resetEnemCounter()
{
    Vei2 d = enemCounter.getDim();
    for (int i = 0; i < d.x; i++)
        for (int j = 0; j < d.y; j++)
            enemCounter.setBlock(i, j, 0);
    nEnemies = 20;
}

void Hud::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    switch (curScreen)
    {
    case Screen::pauseScreen:
        target.draw(pause);
    case Screen::gameOver:
        // this is not a beautiful thing but the playScreen must
        // be drawn on pause and gameover screens
        if(curScreen == Screen::gameOver)
            target.draw(gameOver);
    case Screen::playScreen:
        target.draw(edges1); // drawn on screen origin (ignores ScreenArea translation) 
        target.draw(hudback, states);
        target.draw(lifesholder, states);
        target.draw(levelholder, states);
        target.draw(lifes, states);
        target.draw(level, states);
        target.draw(enemCounter, states);
        break;
    case Screen::construct:
        target.draw(edges1);
        target.draw(hudback, states);
        target.draw(tilePicker, states);
        target.draw(tileSel, states);
        break;
    case Screen::nextStage:
    case Screen::selectStage:
        target.draw(stage);
        target.draw(stageNumText);
        break;
    case Screen::startScreen:
        target.draw(title);
        target.draw(selecter);
        break;
    }
}

void Hud::toggleSelect()
{
    if (selected == 0) {
        selected = 1;
        selecter.setPosition({ 200.f, 430.f });
    }
    else {
        selected = 0;
        selecter.setPosition({ 200.f, 390.f });
    }
}

int Hud::getSelected() const
{
    return selected;
}

void Hud::prevStage()
{
    if (stageNum > 1) {
        stageNum--;
        stageNumText.setNumber(stageNum);
        level.setNumber(stageNum);
    }
}

void Hud::nextStage()
{
    if (stageNum < numStages) {
        stageNum++;
        stageNumText.setNumber(stageNum);
        level.setNumber(stageNum);
    }
}

int Hud::getSelStage() const
{
    return stageNum;
}

void Hud::update(float dt)
{
    switch (curScreen)
    {
    case Screen::pauseScreen:
        pause.update(dt);
        break;
    case Screen::gameOver: {
        Vec2 pos = gameOver.getPosition();
        if (pos.y > Gfx::ScreenHeight / 2.f) {
            gameOver.move({ 0.f, dt * -200 });
        }
        break;
    }
    case Screen::nextStage:
        if (curScreenTime > screenTime) {
            endScreenTime = true;
        }
        curScreenTime += dt;
        break;
    }
}

void Hud::resetGameOverPos()
{
    gameOver.setPosition({ Gfx::ScreenWidth / 2, Gfx::ScreenHeight + 20.f });
}

bool Hud::canGoNextStage()
{
    return endScreenTime;
}

void Hud::resetTime()
{
    endScreenTime = false;
    curScreenTime = 0.f;
}

int Hud::pickTile(Vec2 pos)
{
    return tilePicker.GetBlock(pos);
}

Vec2 Hud::getPickerPos()
{
    return tilePicker.getPosition();
}

void Hud::setSelecTile(int tile)
{
    
    tilePicker.setBlock(0, startSelec, tile);
    tilePicker.setBlock(0, startSelec + 1, tile);
    tilePicker.setBlock(1, startSelec, tile);
    tilePicker.setBlock(1, startSelec + 1, tile);
}
