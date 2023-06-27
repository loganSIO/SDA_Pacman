#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>

// Function declarations
void init();
void draw();
void handleInput(SDL_Event* event);
void gameLoop();

#endif /* GAME_H */
