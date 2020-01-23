#include "Hud.h"
#include "Graphics.h"

Hud::Hud(Texture& tex)
    :
    tex(tex),
    lifesholder(tex, {184, 136, 16, 16}, 1, 0.f),
    levelholder(tex, {184, 184, 16, 15}, 1, 0.f),
    lifes(1, tex),
    level(2, tex),
    enemCounter({2, 10}, Gfx::TileSize, tex,
        Gfx::TextureResolution, {128, 192}, 1)
{
    // screen edges
    edges1.setSize({ Gfx::ScreenWidth-40.f, Gfx::ScreenHeight });
    edges1.setFillColor(Color::Transparent);
    edges1.setOutlineColor(Color(99, 99, 99));
    edges1.setOutlineThickness(-Gfx::EdgeSize);
    hudback.setSize({ 60.f, Gfx::ScreenHeight });
    hudback.setFillColor(Color(99, 99, 99));

    lifesholder.setPosition({ 20.f, 355.f });
    levelholder.setPosition({ 20.f, 480.f });
    lifes.setPosition({ 21.f, 358.f });
    level.setPosition({ 1.f, 507.f });
    enemCounter.setPosition({0.f, 50.f});
    resetEnemCounter();
}

void Hud::setLifes(int nlifes)
{
    lifes.setNumber(nlifes);
}

void Hud::setLevel(int nlevel)
{
    level.setNumber(nlevel);
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
    target.draw(edges1); // drawn on screen origin (ignores ScreenArea translation) 
    target.draw(hudback, states);
    target.draw(lifesholder, states);
    target.draw(levelholder, states);
    target.draw(lifes, states);
    target.draw(level, states);
    target.draw(enemCounter, states);
}
