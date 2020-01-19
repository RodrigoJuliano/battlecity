#include "Game.h"
#include "Graphics.h"
#include <iostream>

Game::Game(RenderWindow& mWindow)
    :
    mWindow(mWindow),
    grnd(texture),
    area_grnd(mWindow, Vec2(Gfx::EdgeSize, Gfx::EdgeSize)),
    rng(rd())
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
        if (player->tryFire()) {
            bullets.emplace_front(new Bullet(texture, { 131,102, 3,4 }, player->GetDirection() * 200.f), player);
            bullets.front().first->setPosition(player->getPosition());
        }
    }
    firePressed = firePressed_new;

    bool spawnEnPressed_new = Keyboard::isKeyPressed(Keyboard::LControl);
    if (!spawnEnPressed && spawnEnPressed_new) {
        enemies.emplace_front(new Enemy(90, texture, { 0,0,13,13 }, rng));
        enemies.front()->setPosition({ 100.f, 100.f });
        enemies.front()->setVel({ 150.f,0.f });
    }
    spawnEnPressed = spawnEnPressed_new;

    player->update(dt, grnd);

    for (auto it = explosions.begin(); it != explosions.end(); ++it) {
        (*it)->update(dt, grnd);
        if ((*it)->finishedAnim()) {
            delete *it;
            it = explosions.erase(it);
        }
        // on erasing item 'it' advance to next position
        // then need to check if reached end before calling ++it
        if (it == explosions.end())
            break;
    }

    // func aux to code below
    auto dec_or_del = [](Tank* t) {
        if (t->getFireCount() == 1) {
            Enemy* e = dynamic_cast<Enemy*>(t);
            if (e && e->getExploded())
                delete e;
            else
                t->decFireCount();
        }
    };

    // When a enimy is deleted it can have a bullet on the world
    // To don't loose the bullet owner info the owner will be deleted
    // only when the bullet get deleled (bullets keeps the reference).
    for (auto it = bullets.begin(); it != bullets.end(); ++it) {
        it->first->update(dt, grnd);

        // Test bullets colling with ground
        if (it->first->Collided()) {
            // Create an explosion
            explosions.emplace_front(new Explosion(texture, { 64, 128, 16, 16 }, 3));
            explosions.front()->setPosition(it->first->getPosition());
            // When an enimy bullet is deleted need to check if the
            // bullet owner was already exploded
            dec_or_del(it->second);
            delete it->first;
            it = bullets.erase(it);
        }
        else if (it->second == player) {
            bool deleted = false;

            // Teste player bullets colliding with enemies
            for (auto ite = enemies.begin(); ite != enemies.end(); ++ite) {
                if (it->first->getCollisionBox().intersects((*ite)->getCollisionBox())) {
                    it->second->decFireCount();
                    // Create an explosion
                    explosions.emplace_front(new Explosion(texture, { 64, 128, 16, 16 }, 3));
                    explosions.front()->setPosition((*ite)->getPosition());
                    // only delete the enemy if don't have bullet (but remove it from enimies)
                    // if have bullet it will be deleted when the bullet get deleted
                    if ((*ite)->getFireCount() == 0)
                        delete* ite;
                    else
                        (*ite)->setExploded();
                    delete it->first;
                    ite = enemies.erase(ite);
                    it = bullets.erase(it);
                    deleted = true;
                    break; // a bullet can only explode one tank
                }
            }
            // Test player bullets colliding with enemies bullets
            if (!deleted) {
                auto cb = it->first->getCollisionBox();
                for (auto itb = next(it); itb != bullets.end(); ++itb) {
                    if (itb->second != player && cb.intersects(itb->first->getCollisionBox())) {
                        
                        dec_or_del(itb->second);
                        it->second->decFireCount();
                        // both bullets need to be deleted
                        delete it->first;
                        delete itb->first;
                        itb = bullets.erase(itb);
                        it = bullets.erase(it);
                        break; // a bullet can only explode one bullet
                    }
                }
            }
        }
        else { // enimy bullet
            if (it->first->getCollisionBox().intersects(player->getCollisionBox())) {
                // kill the player
                player->setPosition(300.f, 400.f);

                dec_or_del(it->second);
                delete it->first;
                it = bullets.erase(it);
            }
        }

        if (it == bullets.end())
            break;
    }

    for (auto e : enemies) {
        e->update(dt, grnd);
        if (e->tryFire()) {
            bullets.emplace_front(new Bullet(texture, { 131,102, 3,4 }, e->GetDirection() * 200.f), e);
            bullets.front().first->setPosition(e->getPosition());
        }
    }

    grnd.update(dt);
}

void Game::draw()
{
    mWindow.clear(Color::Black);
    mWindow.draw(edges);

    area_grnd.draw(*player);
    for (auto e : enemies)
        area_grnd.draw(*e);
    area_grnd.draw(grnd);
    for (auto& e : bullets)
        area_grnd.draw(*(e.first));
    for (auto e : explosions)
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
