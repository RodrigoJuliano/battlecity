#include "Game.h"
#include "Graphics.h"
#include <iostream>
#include <string>
#include <fstream>
#include "Kbd.h"

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
    enemyBonusMarkDist(0, 10),
    bonusTypeDist(0, 5),
    bonusPosDist(grnd.getTileSize()*grnd.getDim().x/2.f,100.0f),
    curScreen(Screen::startScreen),
    hud(texture, curScreen, 35),
    falcon(texture, {64, 16, 16, 14}, 2, 32.f, 0.f)
{
    texture.loadFromFile("resources\\textures.png");

    int bs = grnd.getTileSize();
    pSpawnPos = Vec2(bs * 9.f, bs * 25.f);

    player = new Player(texture, { 0,0,13,13 }, {64,144, 16,16});
    player->addShield(4.f);

    pSpawner = new TankSpawner(texture, player, 1.2f, pSpawnPos);

    // Load sounds
    soundSys.loadSounds("resources\\sounds");

    falcon.setPosition({13.f * bs,25.f * bs });
}

void Game::update(float dt)
{
    switch (curScreen)
    {
    case Screen::construct: {

        // Ground editing

        // Mouse pos relative to ground
        Vec2 mp_grnd = Vec2(Mouse::getPosition(mWindow)) - area_grnd.getOrigin();
        // Mouse pos relative to tilePicker
        Vec2 mp_picker = Vec2(Mouse::getPosition(mWindow)) - area_hud.getOrigin()
            - hud.getPickerPos();
        if (Mouse::isButtonPressed(Mouse::Button::Right)) {
            grnd.setBlock(mp_grnd, -1); // the pos is checkeed inside setBlock
        }
        if (Mouse::isButtonPressed(Mouse::Button::Left)) {
            grnd.setBlock(mp_grnd, id);
            // change the selected tile
            int pick = hud.pickTile(mp_picker);
            if (pick >= 0) {
                id = pick;
                hud.setSelecTile(id);
            }
        }

        if (Kbd::startedPressKey(KbdKey::F1)) {
            grnd.saveToFile("Map.txt"); // will save with a default numbers of tanks
        }

        if (Kbd::startedPressKey(KbdKey::I)) {
            curScreen = Screen::selectStage;
            customMap = true;
        }

        // update ground
        grnd.update(dt);
        break;
    }
    case Screen::playScreen: {

        // Player update
        bool pmovesound_new = true;
        if (!player->isSpawning()) {
            if (Kbd::isKeyPressed(KbdKey::W)) {
                player->setVel({ 0.0f, -100.0f });
            }
            else if (Kbd::isKeyPressed(KbdKey::S)) {
                player->setVel({ 0.0f, 100.0f });
            }
            else if (Kbd::isKeyPressed(KbdKey::A)) {
                player->setVel({ -100.0f, 0.0f });
            }
            else if (Kbd::isKeyPressed(KbdKey::D)) {
                player->setVel({ 100.0f, 0.0f });
            }
            else {
                player->setVel({ 0.0f, 0.0f });
                pmovesound_new = false;
            }
            // Player fire
            if (Kbd::startedPressKey(KbdKey::J)) {
                if (player->tryFire()) {
                    bullets.emplace_front(new Bullet(texture, { 131,102, 3,4 },
                        player->GetDirection() * player->getBulletSpeed(), player->getNumStars() > 2), player);
                    bullets.front().first->setPosition(player->getPosition());
                    soundSys.play(SFX::shoot);
                }
            }

            if (player->getHealth() > 0)
                player->update(dt, grnd);
            else {
                explosions.emplace_front(new Explosion(texture, { 112, 128, 32, 32 }, 2, 0.1f));
                explosions.front()->setPosition(player->getPosition());
                if (player->getNumLifes() > 0) {
                    player->decNumLifes();
                    resetPlayer();
                }
                else {
                    curScreen = Screen::gameOver;
                    soundSys.pause(SFX::tankMove);
                    soundSys.pause(SFX::tankIdle);
                    soundSys.play(SFX::gameOver);
                }
                soundSys.play(SFX::playerExplode);
                pmovesound = false;
            }
        }
        else {
            pmovesound_new = false;
            pSpawner->update(dt);
            if (pSpawner->mustSpawn())
                player->setSpawning(false);
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
    }
    case Screen::gameOver: {
        // this is inside gameScreen so we need to check
        if (curScreen == Screen::gameOver) {
            hud.update(dt);
            if (Kbd::startedPressKey(KbdKey::I)) {
                curScreen = Screen::startScreen;
                hud.resetGameOverPos();
                return;
            }
        }
    }
    { // playScreen
        // Update explosions
        for (auto it = explosions.begin(); it != explosions.end(); ++it) {
            (*it)->update(dt);
            if ((*it)->finishedAnim()) {
                delete* it;
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
            // Check bullet colliding with the falcon
            if (it->first->getCollisionBox().intersects(falcon.getCollisionBox())) {
                falcon.setFrame(1);
                soundSys.pause(SFX::tankIdle);
                soundSys.pause(SFX::tankMove);
                soundSys.play(SFX::playerExplode);
                curScreen = Screen::gameOver;
                pmovesound = false;
                // Create an explosion
                explosions.emplace_front(new Explosion(texture, { 112, 128, 32, 32 }, 2, 0.1f));
                explosions.front()->setPosition(falcon.getPosition());
                delete it->first;
                it = bullets.erase(it);
            }
            // Test bullets colling with ground
            else if (it->first->Collided()) {
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

        // Update spawners
        if (spawners.size() > 0) {
            auto sp_it = spawners.end();
            do {
                sp_it--;
                TankSpawner* ts = *sp_it;
                ts->update(dt);
                if (ts->mustSpawn()) {
                    enemies.push_front(dynamic_cast<Enemy*>(ts->getTank()));
                    delete ts;
                    sp_it = spawners.erase(sp_it);
                }
            } while (sp_it != spawners.begin());
        }

        if (Kbd::startedPressKey(KbdKey::I)) {
            curScreen = Screen::pauseScreen;
            if (soundSys.isPlaying(SFX::tankIdle))
                soundSys.pause(SFX::tankIdle);
            if (soundSys.isPlaying(SFX::tankMove))
                soundSys.pause(SFX::tankMove);
            if (soundSys.isPlaying(SFX::startGame))
                soundSys.pause(SFX::startGame);
        }

        // Stage complete
        if (spawnedEnemies == totalEnemies && enemies.size() == 0) {
            hud.nextStage();
            soundSys.pause(SFX::tankMove);
            soundSys.pause(SFX::tankIdle);
            curScreen = Screen::nextStage;
            pmovesound = false;
        }
        break;
    }
    case Screen::pauseScreen: {
        hud.update(dt);
        if (Kbd::startedPressKey(KbdKey::I)) {
            curScreen = Screen::playScreen;
            soundSys.play(SFX::tankIdle, true);
            if (soundSys.isPaused(SFX::startGame))
                soundSys.play(SFX::startGame);
        }
        break;
    }
    case Screen::nextStage: {
        hud.update(dt);
        if (hud.canGoNextStage()) {
            hud.resetTime();
            curScreen = Screen::playScreen;
            player->setPosition(pSpawnPos);
            player->addShield(4.f);
            pSpawner->reset();
            player->setSpawning();
            resetGame();
            loadLevel(hud.getSelStage());
            hud.resetEnemCounter();
            soundSys.play(SFX::startGame);
            soundSys.play(SFX::tankIdle, true);
            player->resetFireCounter();
        }
        break;
    }
    case Screen::selectStage: {
        if (Kbd::startedPressKey(KbdKey::I)) {
            curScreen = Screen::playScreen;
            if (customMap) {
                // default number of tanks for custom maps
                nBasicTank = 8;
                nSpeedTank = 5;
                nPowerTank = 4;
                nArmorTank = 3;
                customMap = false;
                // Set player spawn and the falcon pos to empty tiles
                // falcon
                grnd.setBlock(12, 24, -1);
                grnd.setBlock(12, 25, -1);
                grnd.setBlock(13, 24, -1);
                grnd.setBlock(13, 25, -1);
                // player spawn
                grnd.setBlock(8, 24, -1);
                grnd.setBlock(8, 25, -1);
                grnd.setBlock(9, 24, -1);
                grnd.setBlock(9, 25, -1);
            }
            else
                loadLevel(hud.getSelStage());
            player->setNumLifes(2);
            resetPlayer();
            hud.resetEnemCounter();
            soundSys.play(SFX::startGame);
            soundSys.play(SFX::tankIdle, true);
            resetGame();
            falcon.setFrame(0);
        }

        if (Kbd::startedPressKey(KbdKey::S)) {
            hud.prevStage();
        }

        if (Kbd::startedPressKey(KbdKey::W)) {
            hud.nextStage();
        }

        break;
    }
    case Screen::startScreen: {
        if (Kbd::startedPressKey(KbdKey::I)) {
            if (hud.getSelected() == 0)
                curScreen = Screen::selectStage;
            else
                curScreen = Screen::construct;
        }

        if (Kbd::startedPressKey(KbdKey::U)) {
            hud.toggleSelect();
        }
        break;
    }
    }
}

void Game::draw()
{
    mWindow.clear(Color::Black);

    switch (curScreen)
    {
    case Screen::pauseScreen:
    case Screen::playScreen:
    case Screen::gameOver:
        area_grnd.draw(grnd);
        area_grnd.draw(falcon);
        if (player->isSpawning())
            area_grnd.draw(*pSpawner);
        else
            area_grnd.draw(*player);
        for (auto e : enemies)
            area_grnd.draw(*e);
        for (auto& e : bullets)
            area_grnd.draw(*(e.first));
        grnd.drawTrees(area_grnd);
        for (auto e : explosions)
            area_grnd.draw(*e);
        if (bonus)
            area_grnd.draw(*bonus);
        for (auto s : spawners)
            area_grnd.draw(*s);
        break;
    case Screen::nextStage:
    case Screen::selectStage:
        mWindow.clear(Color(99, 99, 99));
        break;
    case Screen::construct:
        area_grnd.draw(grnd);
        area_grnd.draw(falcon);
        break;
    //case Screen::startScreen:
    }

    area_hud.draw(hud);

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
    if (spawnedEnemies < totalEnemies && int(enemies.size() + spawners.size()) < maxEnemies) {
        if (enemySpawnDist(rng) == 0) {
            // calc spawn position
            float halfgrnd = (grnd.getTileSize() * grnd.getDim().x) / 2.f;
            float x = 16.f + enemyPosDist(rng) * (halfgrnd-16.f);
            // calc type
            discrete_distribution<int> typedist { (double)nBasicTank,
                (double)nSpeedTank, (double)nPowerTank, (double)nArmorTank};
            int type = typedist(rng);
            float speed = 80.f;
            int health = 1;
            float bulletspeed = 330.f;

            switch (type)
            {
            case 0: // basic
                speed = 70.f;
                bulletspeed = 250.f;
                nBasicTank--;
                break;
            case 1: // speed
                speed = 130.f;
                nSpeedTank--;
                break;
            case 2: // power
                bulletspeed = 500.f;
                nPowerTank--;
                break;
            case 3: // armor
                health = 4;
                nArmorTank--;
                break;
            }
            // spawn
            Enemy* enem = new Enemy(texture, { 0, 64 + 16 * type,13,16 }, rng, health, bulletspeed);
            enem->setVel({ 0.f,speed });
            spawnedEnemies++;
            if (enemyBonusMarkDist(rng) > 9) {
                enem->setBonusMark();
            }
            spawners.emplace_front(new TankSpawner(texture, enem, 1.2f, { x, 16.f }));
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

void Game::loadLevel(int level)
{
    ifstream in("resources\\levels\\level" + to_string(level) + ".txt");
    if (in.fail()) {
        cout << "Error opening file." << endl;
        exit(-1);
    }
    in >> nBasicTank
        >> nSpeedTank
        >> nPowerTank
        >> nArmorTank;

    grnd.loadFromStream(in);
    in.close();
}

void Game::resetPlayer()
{
    player->setPosition(pSpawnPos);
    player->addShield(4.f);
    player->resetStars();
    player->setHealth(1);
    pSpawner->reset();
    player->setSpawning();
    hud.setLifes(player->getNumLifes());
    player->resetFireCounter();
}

void Game::resetGame()
{
    for (auto e : bullets)
        delete e.first;
    for (auto e : enemies)
        delete e;
    for (auto e : explosions)
        delete e;
    if (bonus) {
        delete bonus;
        bonus = nullptr;
    }
    for (auto s : spawners)
        delete s;
    bullets.clear();
    enemies.clear();
    explosions.clear();
    spawners.clear();
    spawnedEnemies = 0;
    pmovesound = false;
}

Game::~Game()
{
    resetGame();
    delete player;
}