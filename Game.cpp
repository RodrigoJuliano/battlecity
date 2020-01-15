#include "Game.h"
#include "Graphics.h"
#include <iostream>

Game::Game(RenderWindow& mWindow)
    :
    mWindow(mWindow)
{
    texture.loadFromFile("textures.png");

    loadBlocks();

    //player = new Entity(0, texture, { 0,16*3,16,16 }, { 400.f, 400.f });
    //player->setOrigin({ Gfx::TextureResolution * 0.5f , Gfx::TextureResolution * 0.5f  });
    //player->setScale(Gfx::TextureScaleMult, Gfx::TextureScaleMult);

    player = new Tank(0, texture, { 32,1,15,15 }, { 400.f, 400.f });
    player->setOrigin({ 15 * 0.5f, 15 * 0.5f});
    player->setScale(Gfx::TextureScaleMult, Gfx::TextureScaleMult);

}

void Game::update(float dt)
{

    Vec2 mp = Vec2(Mouse::getPosition(mWindow));

    if (Mouse::isButtonPressed(Mouse::Right)) {
        grnd.setBlock(mp, nullptr);
    }

    if (Mouse::isButtonPressed(Mouse::Left)) {
        grnd.setBlock(mp, Block::mBlocks[id]);
    }

    bool toggleBlockPressed_new = Keyboard::isKeyPressed(Keyboard::LControl);
    if(!toggleBlockPressed && toggleBlockPressed_new){
        if (id == 13)
            id = 0;
        else
            id++;
        cout << "Cur block: " << id << endl;
    }
    toggleBlockPressed = toggleBlockPressed_new;


    if (Keyboard::isKeyPressed(Keyboard::W)) {
        player->setVel({0.0f, -150.0f});
    }
    else if (Keyboard::isKeyPressed(Keyboard::S)) {
        player->setVel({ 0.0f, 150.0f });
    }
    else if (Keyboard::isKeyPressed(Keyboard::A)) {
        player->setVel({ -150.0f, 0.0f });
    }
    else if (Keyboard::isKeyPressed(Keyboard::D)) {
        player->setVel({ 150.0f, 0.0f });
    }
    else {
        player->setVel({ 0.0f, 0.0f });
    }
    
    bool firePressed_new = Keyboard::isKeyPressed(Keyboard::RShift);
    if (!firePressed && firePressed_new) {
        Vec2 ppos = player->getPosition();
        Bullet* b = new Bullet(texture, {131,102, 3,4}, ppos);
        b->setOrigin({2.f,1.5f});
        b->setScale(Gfx::TextureScaleMult, Gfx::TextureScaleMult);
        b->setVel(player->GetDirection() * 200.f);
        bullets.push_back(b);
    }
    firePressed = firePressed_new; 

    player->update(dt, grnd);

    if (bullets.size() > 40) {
        for (int i = bullets.size()-1; i >= 0; i--) {
            if (bullets.at(i)->Collided()) {
                delete bullets.at(i);
                bullets.erase(bullets.begin() + i);
            }
        }
    }

    for (auto b : bullets)
        if(!b->Collided())
            b->update(dt, grnd);

    grnd.update(dt);
}

void Game::draw()
{
    mWindow.clear(Color::Black);

    mWindow.draw(*player);
    grnd.Draw(mWindow);

    for (auto b : bullets)
        if (!b->Collided())
            mWindow.draw(*b);

    // player collision box
    auto p = player->getCollisionBox();
    RectangleShape r({ p.width, p.height });
    r.setOutlineColor(Color::Magenta);
    r.setOutlineThickness(-1);
    r.setFillColor(Color::Transparent);
    r.setPosition({p.left, p.top});
    mWindow.draw(r);

    mWindow.display();
}


void Game::loadBlocks() {
    int id = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 4; j++) {
            Block::mBlocks[id] = new Block(id, texture, IntRect(64 + 8 * j, 8 * i, 8, 8));
            Block::mBlocks[id]->setScale(Gfx::TextureScaleMult, Gfx::TextureScaleMult);
            id++;
        }
    Block::mBlocks[id] = new Block(id, texture, IntRect(64, 24, 8, 8), 3);
    Block::mBlocks[id]->setScale(Gfx::TextureScaleMult, Gfx::TextureScaleMult);
}