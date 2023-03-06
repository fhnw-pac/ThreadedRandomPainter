// ThreadedRandomPainter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SDLWindow.h"
#include "RandomPainter.h"

int main()
{
    RandomPainter painter;
    SDLWindow win(1600, 900, "noisyImg");

    bool run = true;

    while (run) {

        // handle window events
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)
                run = false;
        }
 
        painter.paint(win.getImageBuffer());
        win.renderFrame();
    }

    return 0;
}
