#include "Game.h"
#include "Graphics.h"
#include <iostream>

Game::Game(RenderWindow& mWindow)
    :
    mWindow(mWindow),
    grnd({ 26,26 }, Gfx::TileSize, texture, Gfx::TextureResolution,
        { 64, 0 }, 4, 3, 0.3f),
    area_grnd(mWindow, Vec2(Gfx::EdgeSize, Gfx::EdgeSize)),
    area_hud(mWindow, Vec2(Gfx::ScreenWidth - 60.f, 0.f)),
    rng(rd()),
    enemyPosDist(0, 2),
    enemySpawnDist(0, 100),
    enemyTypeDist{ 4, 2, 3, 1},
    enemyBonusMarkDist(0, 10),
    bonusTypeDist(0, 5),
    bonusPosDist(grnd.getTileSize()*grnd.getDim().x/2.f,100.0f),
    hud(texture)
{
    texture.loadFromFile("resources\\textures.png");

    grnd.loadFromFile("resources\\level1.txt");

    int bs = grnd.getTileSize();
    pSpawnPos = Vec2(bs * 9.f, bs * 25.f);

    player = new Player(texture, { 0,0,13,13 }, {64,144, 16,16});
    player->setPosition(pSpawnPos);
    player->addShield(4.f);

    // Load sounds
    soundSys.loadSounds("resources\\sounds");
    //soundSys.play(SFX::tankIdle, true);
    soundSys.play(SFX::startGame);

    hud.setLifes(player->getNumLifes());
    hud.setLevel(1);
}

void Game::update(float dt)
{
    // Ground editing
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

    bool spawnEnPressed_new = Keyboard::isKeyPressed(Keyboard::LControl);
    if (!spawnEnPressed && spawnEnPressed_new) {

        grnd.saveToFile("level2.txt");
        //grnd.loadFromFile("level1.txt");
    }
    spawnEnPressed = spawnEnPressed_new;

    // Player move
    bool pmovesound_new = true;
    if (Keyboard::isKeyPressed(Keyboard::W)) {
        player->setVel({0.0f, -100.0f});
    }
    else if (Keyboard::isKeyPressed(Keyboard::S)) {
        player->setVel({ 0.0f, 100.0f });
    }
    else if (Keyboard::isKeyPressed(Keyboard::A)) {
        player->setVel({ -100.0f, 0.0f });
    }
    else if (Keyboard::isKeyPressed(Keyboard::D)) {
        player->setVel({ 100.0f, 0.0f });
    }
    else {
        player->setVel({ 0.0f, 0.0f });
        pmovesound_new = false;
    }

    // toggles between move and idle sounds
    if (pmovesound_new) {
        if (!pmovesound) {
            soundSys.pause(SFX::tankIdle);
            soundSys.play(SFX::tankMove, true);
        }
    }
    else if (pmovesound) {
        soundSys.pause(SFX::tankMove);
        soundSys.play(SFX::tankIdle, true);
    }
    pmovesound = pmovesound_new;

    // Player fire
    bool firePressed_new = Keyboard::isKeyPressed(Keyboard::RShift);
    if (!firePressed && firePressed_new) {
        if (player->tryFire()) {
            bullets.emplace_front(new Bullet(texture, { 131,102, 3,4 },
                player->GetDirection() * player->getBulletSpeed(), player->getNumStars() > 2), player);
            bullets.front().first->setPosition(player->getPosition());
            soundSys.play(SFX::shoot);
        }
    }
    firePressed = firePressed_new;

    // Update explosions
    for (auto it = explosions.begin(); it != explosions.end(); ++it) {
        (*it)->update(dt);
        if ((*it)->finishedAnim()) {
            delete *it;
            it = explosions.erase(it);
        }
        // on erasing item 'it' advance to next position
        // then need to check if reached end before calling ++it
        if (it == explosions.end())
            break;
    }

    if (explosions.size() > 0) { // At last 1 explosion
        auto ex_it = explosions.end();
        do {
            ex_it--;
            Explosion* ex = *ex_it;
            ex->update(dt);
            if (ex->finishedAnim()) {
                delete ex;
                ex_it = explosions.erase(ex_it);
            }
        } while (ex_it != explosions.begin());
    }


    // Update bullets

    // func aux to code below
    auto dec_or_del = [](Tank* t) {
        t->decFireCount();
        if (t->getFireCount() == 1) {
            Enemy* e = dynamic_cast<Enemy*>(t);
            if (e && e->getHealth() < 1)
                delete e;
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
            if (it->second == player)
                soundSys.play(SFX::bulletHit);
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
                Enemy* enem = *ite;
                if (it->first->getCollisionBox().intersects((*ite)->getCollisionBox())) {
                    player->decFireCount();
                    enem->hit();
                    // check for bonus
                    if (enem->hasBonusMark()) {
                        spawnBonus();
                        enem->setBonusMark(false);
                    }
                    if (enem->getHealth() > 0) {
                        // Bullet explosion
                        explosions.emplace_front(new Explosion(texture, { 64, 128, 16, 16 }, 3));
                        explosions.front()->setPosition(it->first->getPosition());
                        soundSys.play(SFX::bulletHitTank);
                    }

                    delete it->first;
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
        else { // enemy bullet
            if (it->first->getCollisionBox().intersects(player->getCollisionBox())) {
                if (!player->isShielded()) {
                    // kill the player
                    player->kill();
                }
                dec_or_del(it->second);
                delete it->first;
                it = bullets.erase(it);
            }
        }

        if (it == bullets.end())
            break;
    }

    // update ground
    grnd.update(dt);

    // Timer bonus
    if (timerBonusOn) {
        timerBonusTime -= dt;
        if (timerBonusTime < 0.f)
            timerBonusOn = false;
    }

    // Shovel bonus
    if (shovelBonusOn) {
        shovelBonusTime -= dt;
        if (shovelBonusTime < 0.f) {
            shovelBonusOn = false;
            setblocksshovelbonus(0);
        }
    }

    if (bonus) {
        bonus->update(dt);
        if (bonus->getTime() < 0.f) {
            delete bonus;
            bonus = nullptr;
        }
        else if (player->getCollisionBox().intersects(bonus->getCollisionBox())) {
            SFX sound = SFX::getBonus;
            switch (bonus->getType())
            {
            case Bonus::Type::helmet:
                player->addShield(10.f);
                break;
            case Bonus::Type::timer:
                timerBonusOn = true;
                timerBonusTime = 15.f;
                break;
            case Bonus::Type::shovel:
                shovelBonusOn = true;
                shovelBonusTime = 15.f;
                setblocksshovelbonus(1);
                break;
            case Bonus::Type::star:
                player->addStar();
                break;
            case Bonus::Type::grenade:
                for (auto e : enemies)
                    e->kill();
                break;
            case Bonus::Type::tank:
                player->addLife();
                sound = SFX::getLife;
                hud.setLifes(player->getNumLifes());
                break;
            }
            delete bonus;
            bonus = nullptr;
            soundSys.play(sound);
        }
    }

    // Update enemies
    if (enemies.size() > 0) { // At last 1 enemy
        auto en_it = enemies.end();
        do {
            en_it--;
            Enemy* en = *en_it;
            if (en->getHealth() > 0) {
                if (!timerBonusOn) {
                    en->update(dt, grnd);
                    if (en->tryFire()) {
                        bullets.emplace_front(new Bullet(texture, { 131,102, 3,4 },
                            en->GetDirection() * en->getBulletSpeed()), en);
                        bullets.front().first->setPosition(en->getPosition());
                    }
                }
            }
            else {
                // Tank explosion
                explosions.emplace_front(new Explosion(texture, { 112, 128, 32, 32 }, 2, 0.1f));
                explosions.front()->setPosition(en->getPosition());
                soundSys.play(SFX::tankExplode);

                if (en->getFireCount() == 0)
                    delete en;
                en_it = enemies.erase(en_it);
            }
        } while (en_it != enemies.begin());
    }
    ctrlNumEnemies();

    // Player tank update
    if(player->getHealth() > 0)
        player->update(dt, grnd);
    else {
        explosions.emplace_front(new Explosion(texture, { 112, 128, 32, 32 }, 2, 0.1f));
        explosions.front()->setPosition(player->getPosition());
        player->setPosition(pSpawnPos);
        player->addShield(4.f);
        player->resetStars();
        player->setHealth(1);
        player->decNumLifes();
        soundSys.play(SFX::playerExplode);
        hud.setLifes(player->getNumLifes());
    }
}

void Game::draw()
{
    mWindow.clear(Color::Black);
    area_hud.draw(hud);

    area_grnd.draw(*player);
    for (auto e : enemies)
        area_grnd.draw(*e);
    area_grnd.draw(grnd);
    for (auto& e : bullets)
        area_grnd.draw(*(e.first));
    for (auto e : explosions)
        area_grnd.draw(*e);
    if (bonus)
        area_grnd.draw(*bonus);

    // player collision box
    //auto p = player->getCollisionBox();
    //RectangleShape r({ p.width, p.height });
    //r.setOutlineColor(Color::Magenta);
    //r.setOutlineThickness(-1);
    //r.setFillColor(Color::Transparent);
    //r.setPosition({p.left, p.top});
    //area_grnd.draw(r);

    mWindow.display();
}

void Game::ctrlNumEnemies()
{
    // check if can spawn an enemy
    if (spawnedEnemies < totalEnemies && int(enemies.size()) < maxEnemies) {
        if (enemySpawnDist(rng) == 0) {
            // calc spawn position
            float halfgrnd = (grnd.getTileSize() * grnd.getDim().x) / 2.f;
            float x = 16.f + enemyPosDist(rng) * (halfgrnd-16.f);
            // calc type
            int type = enemyTypeDist(rng);
            float speed = 70.f;
            int health = 1;
            float bulletspeed = 330.f;
            if (type == 0) {
                speed = 60.f;
                bulletspeed = 250.f;
            }
            else if (type == 1) {
                speed = 125.f;
                bulletspeed = 500.f;
            }
            else if (type == 3)
                health = 4;
            // spawn
            enemies.emplace_front(new Enemy(texture, { 0, 64 + 16*type,13,16 }, rng, health, bulletspeed));
            enemies.front()->setPosition({x, 16.f});

            enemies.front()->setVel({ 0.f,speed });
            spawnedEnemies++;

            if (enemyBonusMarkDist(rng) > 9) {
                enemies.front()->setBonusMark();
            }

            hud.removeEnemy();
        }
    }
}

void Game::setblocksshovelbonus(int block)
{
    // left
    grnd.setBlock(11, 23, block);
    grnd.setBlock(11, 24, block);
    grnd.setBlock(11, 25, block);
    // top
    grnd.setBlock(12, 23, block);
    grnd.setBlock(13, 23, block);
    // right
    grnd.setBlock(14, 23, block);
    grnd.setBlock(14, 24, block);
    grnd.setBlock(14, 25, block);
}

void Game::spawnBonus()
{
    int type = bonusTypeDist(rng);
    if (bonus) // only one bonus is allowed
        delete bonus;
    bonus = new Bonus(texture, { 96, 15 * type, 16,15 }, Bonus::Type(type));
    int grnsz = grnd.getTileSize() * grnd.getDim().x;
    float x = bonusPosDist(rng);
    float y = bonusPosDist(rng);
    // clamp the position
    x = min(max(x, 16.f), grnsz - 16.f);
    y = min(max(y, 16.f), grnsz - 16.f);
    bonus->setPosition({ x, y });
    soundSys.play(SFX::bonusSpawn);
}

Game::~Game()
{
    for (auto e : bullets)
        delete e.first;
    for (auto e : enemies)
        delete e;
    for (auto e : explosions)
        delete e;
    if (bonus)
        delete bonus;
    delete player;
}