
#include "Game.h"
#include "Graphics.h"

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
       /* if (!mWindow.hasFocus())
            continue;*/

        Time dt = clock.restart();
        float dtAsSeconds = dt.asSeconds();
        game.update(dtAsSeconds);
        game.draw();
    }
    return 0;
}
