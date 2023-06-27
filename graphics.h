#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <stdbool.h>

// Constants
#define NUM_WALLS 47
#define NUM_PELLETS 192
#define NUM_BIG_PELLETS 4

typedef struct {
    SDL_Rect rect;
    SDL_Rect sprites[4];
    int direction;
    int count;
} Ghost;

// Pacgum definition
typedef struct {
    SDL_Rect rect;
    bool eaten;
} Pellet;

// Super pacgum definition
typedef struct {
   SDL_Rect rect;
   bool eaten;
} BigPellet;

// Window and Surfaces
extern SDL_Window* pWindow;
extern SDL_Surface* win_surf;
extern SDL_Surface* plancheSprites;

extern SDL_Rect src_bg;
extern SDL_Rect bg;
extern SDL_Rect welcome_title;

extern Ghost ghost_red;
extern Ghost ghost_pink;
extern Ghost ghost_cyan;
extern Ghost ghost_orange;

extern SDL_Rect pacman;
extern SDL_Rect pacman_closed;
extern SDL_Rect pacman_right;
extern SDL_Rect pacman_left;
extern SDL_Rect pacman_up;
extern SDL_Rect pacman_down;
extern SDL_Rect pacman_wide_right;
extern SDL_Rect pacman_wide_left;
extern SDL_Rect pacman_wide_up;
extern SDL_Rect pacman_wide_down;

extern SDL_Rect pacman_death1;
extern SDL_Rect pacman_death2;
extern SDL_Rect pacman_death3;
extern SDL_Rect pacman_death4;
extern SDL_Rect pacman_death5;
extern SDL_Rect pacman_death6;
extern SDL_Rect pacman_death7;
extern SDL_Rect pacman_death8;
extern SDL_Rect pacman_death9;
extern SDL_Rect pacman_death10;

// Super pacgum
extern SDL_Rect bigPellet;

extern SDL_Rect letters[26];
extern SDL_Rect numbers[10];
extern SDL_Rect tiret;
extern SDL_Rect exclamation;
extern SDL_Rect espace;

extern int lifes;
extern int score;
extern int highscore;
extern bool isPelletEaten;
extern bool isBigPelletEaten;
extern bool gameWon;
extern bool gameLost;
extern bool isWideOpen;

extern Uint32 lastAnimationChangeTime;
extern int animationDelay;
extern int lastDirection;
extern SDL_Rect walls[NUM_WALLS];
extern Pellet pellets[NUM_PELLETS];
extern BigPellet bigPellets[NUM_BIG_PELLETS];

// Functions
void display_highscore_title();
void display_score_title();
void display_game_score();
void display_game_highscore();
void display_lifes(int pacman_lifes_count);
void draw_all_pellets();

void animatePacmanDeath(SDL_Surface* win_surf, SDL_Surface* plancheSprites, int numFrames, SDL_Rect PacmanPos, Uint32 animationDelay);
void drawStartMenu(SDL_Surface* surface);
void drawEndMenu(SDL_Surface* surface, bool gameLost);

#endif