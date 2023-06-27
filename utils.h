#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <stdbool.h>

// Function declarations
void respawn();
void CheckCollisionWithGhost(SDL_Rect pacman);
bool checkWallCollision(SDL_Rect rect);
bool checkPelletCollision(SDL_Rect rect);
bool checkCollision(SDL_Rect rect);
void moveGhosts();
void updatePacman();
void restartGame();

#endif
