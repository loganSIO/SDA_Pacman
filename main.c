#include <SDL2/SDL.h>
#include <unistd.h>
#include <libgen.h>
#include <stdio.h>
#include <stdbool.h>

#include "game.h"
#include "graphics.h"
#include "utils.h"

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL initialization failed: %s", SDL_GetError());
        return 1;
    }

    init();

    gameLoop();

    SDL_DestroyWindow(pWindow);
    SDL_Quit();

    return 0;
}