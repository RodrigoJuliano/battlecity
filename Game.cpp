#include "Game.h"
#include "Graphics.h"
#include <iostream>

Game::Game(RenderWindow& mWindow)
    :
    mWindow(mWindow),
    grnd(texture),
    area_grnd(mWindow, Vec2(Gfx::EdgeSize, Gfx::EdgeSize))
{
    texture.loadFromFile("textures.png");

    player = new Tank(0, texture, { 0,0,13,13 });
    player->setPosition({ 100.f, 100.f });

    // screen edges
    edges.setSize({ Gfx::ScreenWidth, Gfx::ScreenHeight });
    edges.setPosition(0.f, 0.f);
    edges.setFillColor(Color::Transparent);
    edges.setOutlineColor(Color(99, 99, 99));
    edges.setOutlineThickness(-Gfx::EdgeSize);
}

void Game::update(float dt)
{

    Vec2 mp = Vec2(Mouse::getPosition(mWindow)) - area_grnd.getOrigin();
    if (Mouse::isButtonPressed(Mouse::Right)) {
        grnd.setBlock(mp, -1);
    }

    if (Mouse::isButtonPressed(Mouse::Left)) {
        grnd.setBlock(mp, id);
    }

    bool toggleBlockPressed_new = Keyboard::isKeyPressed(Keyboard::LControl);
    if(!toggleBlockPressed && toggleBlockPressed_new){
        if (id == 12)
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
        entities.emplace_front(new Bullet(texture, { 131,102, 3,4 }, player->GetDirection() * 200.f));
        entities.front()->setPosition(player->getPosition());
    }
    firePressed = firePressed_new;

    player->update(dt, grnd);

    for (auto it = entities.begin(); it != entities.end(); ++it) {
        (*it)->update(dt, grnd);
        if (Explosion* e = dynamic_cast<Explosion*>(*it)) {
            if (e->finishedAnim()) {
                delete e;
                it = entities.erase(it);
            }
        }
        else if (Bullet* b = dynamic_cast<Bullet*>(*it)) {
            if (b->Collided()) {
                entities.emplace_front(new Explosion(texture, { 64, 128, 16, 16 }, 3));
                entities.front()->setPosition(b->getPosition());
                delete b;
                it = entities.erase(it);
            }
        }

        // erasing a item move 'it' to next position
        // then need to check before calling ++it
        if (it == entities.end())
            break;
    }

    grnd.update(dt);
}

void Game::draw()
{
    mWindow.clear(Color::Black);
    mWindow.draw(edges);

    area_grnd.draw(*player);
    area_grnd.draw(grnd);
    for(auto e : entities)
        area_grnd.draw(*e);

    // player collision box
    auto p = player->getCollisionBox();
    RectangleShape r({ p.width, p.height });
    r.setOutlineColor(Color::Magenta);
    r.setOutlineThickness(-1);
    r.setFillColor(Color::Transparent);
    r.setPosition({p.left, p.top});
    area_grnd.draw(r);

    mWindow.display();
}
