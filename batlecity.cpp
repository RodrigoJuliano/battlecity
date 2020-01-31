
#include "Game.h"
#include "Gfx.h"

int main()
{
    Clock clock;
    RenderWindow mWindow;
    Game game(mWindow);

    mWindow.create(VideoMode(Gfx::ScreenWidth, Gfx::ScreenHeight), "Battle City");
    mWindow.setFramerateLimit(60);

    while (mWindow.isOpen())
    {
        Event e;
        while (mWindow.pollEvent(e))
        {
            if (e.type == Event::Closed)
                mWindow.close();
        }
        Time dt = clock.restart();
        if (!mWindow.hasFocus())
            continue;
        float dtAsSeconds = dt.asSeconds();
        game.update(dtAsSeconds);
        game.draw();
    }
    return 0;
}
