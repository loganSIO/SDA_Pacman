#include <SDL2/SDL.h>
#include <unistd.h>
#include <libgen.h>
#include <stdio.h>
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
SDL_Window* pWindow = NULL;
SDL_Surface* win_surf = NULL;
SDL_Surface* plancheSprites = NULL;

// Background
SDL_Rect src_bg = { 369, 3, 168, 216 };
SDL_Rect bg = { 4, 4, 672, 864 };
SDL_Rect welcome_title = { 4, 4, 180, 46 };

// Ghosts
Ghost ghost_red = { {325, 325, 32, 32}, {{3, 123, 16, 16}, {37, 123, 16, 16}, {105, 123, 16, 16}, {71, 123, 16, 16}}, 0, 0 };
Ghost ghost_pink = { {325, 400, 32, 32}, {{3, 141, 16, 16}, {37, 141, 16, 16}, {105, 141, 16, 16}, {71, 141, 16, 16}}, 1, 0 };
Ghost ghost_cyan = { {325, 425, 32, 32}, {{3, 159, 16, 16}, {37, 159, 16, 16}, {105, 159, 16, 16}, {71, 159, 16, 16}}, 1, 0 };
Ghost ghost_orange = { {325, 380, 32, 32}, {{3, 177, 16, 16}, {37, 177, 16, 16}, {105, 177, 16, 16}, {71, 177, 16, 16}}, 1, 0 };

// Pacman
SDL_Rect pacman = { 325, 646, 32, 32 };
SDL_Rect pacman_closed = { 3, 90, 16, 16 };
SDL_Rect pacman_right = { 20, 90, 16, 16 };
SDL_Rect pacman_left = { 46, 90, 16, 16 };
SDL_Rect pacman_up = { 75, 90, 16, 16 };
SDL_Rect pacman_down = { 109, 90, 16, 16 };
SDL_Rect pacman_wide_right = { 36, 88, 12, 16 };
SDL_Rect pacman_wide_left = { 63, 88, 12, 16 };
SDL_Rect pacman_wide_up = { 93, 90, 16, 16 };
SDL_Rect pacman_wide_down = { 126, 90, 16, 16 };

// Pacman death
SDL_Rect pacman_death1 = { 3, 105, 16, 14 };
SDL_Rect pacman_death2 = { 24, 105, 16, 14 };
SDL_Rect pacman_death3 = { 42, 105, 16, 14 };
SDL_Rect pacman_death4 = { 61, 105, 16, 14 };
SDL_Rect pacman_death5 = { 80, 105, 16, 14 };
SDL_Rect pacman_death6 = { 99, 105, 14, 14 };
SDL_Rect pacman_death7 = { 116, 105, 10, 14 };
SDL_Rect pacman_death8 = { 129, 105, 6, 14 };
SDL_Rect pacman_death9 = { 137, 105, 4, 14 };
SDL_Rect pacman_death10 = { 143, 105, 11, 14 };


// Super pacgum
SDL_Rect bigPellet = { 9, 79, 8, 8 };

// Letters
SDL_Rect letters[26] = {
    { 12, 61, 7, 7 }, { 20, 61, 7, 7 }, { 28, 61, 7, 7 }, { 36, 61, 7, 7 }, { 44, 61, 7, 7 }, { 52, 61, 7, 7 },
    { 60, 61, 7, 7 }, { 68, 61, 7, 7 }, { 76, 61, 7, 7 }, { 84, 61, 7, 7 }, { 92, 61, 7, 7 }, { 100, 61, 7, 7 },
    { 108, 61, 7, 7 }, { 116, 61, 7, 7 }, { 124, 61, 7, 7 }, { 4, 69, 7, 7 }, { 12, 69, 7, 7 }, { 20, 69, 7, 7 },
    { 28, 69, 7, 7 }, { 36, 69, 7, 7 }, { 44, 69, 7, 7 }, { 52, 69, 7, 7 }, { 60, 69, 7, 7 }, { 68, 69, 7, 7 },
    { 76, 69, 7, 7 }, { 84, 69, 7, 7 }
};

// Numbers
SDL_Rect numbers[10] = {
    { 4, 53, 7, 7 }, { 12, 53, 7, 7 }, { 20, 53, 7, 7 }, { 28, 53, 7, 7 }, { 36, 53, 7, 7 },
    { 44, 53, 7, 7 }, { 52, 53, 7, 7 }, { 60, 53, 7, 7 }, { 68, 53, 7, 7 }, { 76, 53, 7, 7 }
};

// Other characters
SDL_Rect tiret = { 84, 53, 7, 7 };
SDL_Rect exclamation = { 101, 52, 7, 7 };
SDL_Rect espace = { 115, 52, 7, 7 };

// Global variables
int lifes = 3;
int score = 0;
int highscore = 0;
bool isPelletEaten = false;
bool isBigPelletEaten = false;
bool gameWon = false;
bool gameLost = false;
bool isWideOpen = false;

Uint32 lastAnimationChangeTime = 0;
int animationDelay = 200;
int lastDirection = -1;

// Wall placement
SDL_Rect walls[NUM_WALLS] = {
   // Sides of the map
   { 0, 0, 1680, 31 },         // Top wall
   { 0, 0, 31, 414},           // Upper left side wall
   { 0, 458, 31, 450},         // Lower left side wall
   { 644, 0, 100, 415},        // Upper right side
   { 644, 458, 100, 450},      // Lower right side wall
   { 0, 864 - 23, 1680, 100 }, // bottom wall

   // Tunnel walls
   { 0, 298, 158, 116 },   // Top of the left tunnel
   { 0, 458, 158, 116 },   // Bottom of the left tunnel
   { 522, 298, 158, 116 }, // Top of the right tunnel
   { 522, 458, 158, 116 }, // Bottom of the right tunnel


   // Walls attached to sides
   { 0, 682, 93, 52 },    // Left
   { 588, 682, 93, 52},   // Right


   //Top left corner blocks
   { 74, 74, 84, 84 },     // First block top left
   { 74, 202, 84, 52 },    // Block under top left
   { 202, 74, 84, 84 },    // Bloc right to top left


   //Top right corner blocks
   { 522, 74, 84, 84 },    // First block top right
   { 522, 202, 84, 52 },   // Block under top right
   { 394, 74, 84, 84 },    // Bloc left to top right


   // Vertical left lines from top to bottom
   { 202, 202, 20, 210},
   { 202, 460, 20, 112 },
   { 138, 638, 20, 94 },
   { 202, 684, 20, 112},


   // Vertical right lines from top to bottom
   { 458, 202, 20, 210 },
   { 458, 460, 20, 112 },  // left one
   { 522, 638, 20, 94 },   // right one
   { 458, 684, 20, 112 },


   // Horizontal left lines
   { 222, 298, 61, 20},  // Line over mid
   { 202, 618, 84, 20 }, // Line under mid
   { 74, 618, 84, 20 },  // Line left
   { 74, 778, 212, 20 }, // Line bottom

   // Horizontal right lines from top to bottom
   { 397, 298, 61, 20 },
   { 522, 618, 84, 20 }, // left one
   { 394, 618, 84, 20 }, // right one
   { 394, 778, 212, 20 },


   // Middle of the map from top to bottom
   { 330, 30, 20, 128 },   // first line
   { 266, 202, 148, 52 },  // first rectangle
   { 330, 214, 20, 102 },  // first rectangle line
   { 266, 522, 148, 52 },  // second rectangle
   { 330, 534, 20, 102 },  // second rectangle line
   { 266, 682, 148, 52 },  // third rectangle
   { 330, 694, 22, 102 },  // third rectangle line


   // Ghost house walls
   { 266, 362, 42, 20},    // Wall left to door
   { 372, 362, 42, 20},    // Wall right to door
   { 266, 362, 20, 112 },  // Left wall
   { 394, 362, 20, 112 },  // Right wall
   { 266, 458, 148, 20 },  // Bottom wall};

};

// Pacgum placement
Pellet pellets[NUM_PELLETS] = {
    { { 176, 48, 8, 8 }, false },
    { { 176, 80, 8, 8 }, false },
    { { 176, 112, 8, 8 }, false },
    { { 176, 144, 8, 8 }, false },
    { { 176, 176, 8, 8 }, false },
    { { 176, 208, 8, 8 }, false },
    { { 176, 240, 8, 8 }, false },
    { { 176, 272, 8, 8 }, false },
    { { 176, 304, 8, 8 }, false },
    { { 176, 336, 8, 8 }, false },
    { { 176, 368, 8, 8 }, false },
    { { 176, 400, 8, 8 }, false },
    { { 176, 432, 8, 8 }, false },
    { { 176, 464, 8, 8 }, false },
    { { 176, 496, 8, 8 }, false },
    { { 176, 528, 8, 8 }, false },
    { { 176, 560, 8, 8 }, false },
    { { 176, 592, 8, 8 }, false },
    { { 176, 624, 8, 8 }, false },
    { { 176, 656, 8, 8 }, false },
    { { 176, 688, 8, 8 }, false },
    { { 176, 720, 8, 8 }, false },
    { { 176, 752, 8, 8 }, false },

    { { 48, 176, 8, 8 }, false },
    { { 80, 176, 8, 8 }, false },
    { { 112, 176, 8, 8 }, false },
    { { 144, 176, 8, 8 }, false },
    { { 176, 176, 8, 8 }, false },
    { { 208, 176, 8, 8 }, false },
    { { 240, 176, 8, 8 }, false },
    { { 272, 176, 8, 8 }, false },
    { { 304, 176, 8, 8 }, false },
    { { 336, 176, 8, 8 }, false },
    { { 368, 176, 8, 8 }, false },
    { { 400, 176, 8, 8 }, false },
    { { 432, 176, 8, 8 }, false },
    { { 464, 176, 8, 8 }, false },
    { { 496, 176, 8, 8 }, false },
    { { 528, 176, 8, 8 }, false },
    { { 560, 176, 8, 8 }, false },
    { { 592, 176, 8, 8 }, false },
    { { 624, 176, 8, 8 }, false },

    { { 48, 48, 8, 8 }, false },
    { { 48, 80, 8, 8 }, false },
    { { 48, 144, 8, 8 }, false },
    { { 48, 176, 8, 8 }, false },
    { { 48, 208, 8, 8 }, false },
    { { 48, 240, 8, 8 }, false },
    { { 48, 272, 8, 8 }, false },

    { { 80, 48, 8, 8 }, false },
    { { 112, 48, 8, 8 }, false },
    { { 144, 48, 8, 8 }, false },
    { { 208, 48, 8, 8 }, false },
    { { 240, 48, 8, 8 }, false },
    { { 272, 48, 8, 8 }, false },
    { { 304, 48, 8, 8 }, false },

    { { 304, 80, 8, 8 }, false },
    { { 304, 112, 8, 8 }, false },
    { { 304, 144, 8, 8 }, false },

    { { 368, 48, 8, 8 }, false },
    { { 400, 48, 8, 8 }, false },
    { { 432, 48, 8, 8 }, false },
    { { 464, 48, 8, 8 }, false },
    { { 496, 48, 8, 8 }, false },
    { { 528, 48, 8, 8 }, false },
    { { 560, 48, 8, 8 }, false },
    { { 592, 48, 8, 8 }, false },
    { { 624, 48, 8, 8 }, false },

    { { 368, 80, 8, 8 }, false },
    { { 368, 112, 8, 8 }, false },
    { { 368, 144, 8, 8 }, false },

    { { 496, 80, 8, 8 }, false },
    { { 496, 112, 8, 8 }, false },
    { { 496, 144, 8, 8 }, false },
    { { 496, 176, 8, 8 }, false },
    { { 496, 208, 8, 8 }, false },
    { { 496, 240, 8, 8 }, false },
    { { 496, 272, 8, 8 }, false },
    { { 496, 304, 8, 8 }, false },
    { { 496, 336, 8, 8 }, false },
    { { 496, 368, 8, 8 }, false },
    { { 496, 400, 8, 8 }, false },
    { { 496, 432, 8, 8 }, false },
    { { 496, 464, 8, 8 }, false },
    { { 496, 496, 8, 8 }, false },
    { { 496, 528, 8, 8 }, false },
    { { 496, 560, 8, 8 }, false },
    { { 496, 592, 8, 8 }, false },
    { { 496, 624, 8, 8 }, false },
    { { 496, 656, 8, 8 }, false },
    { { 496, 688, 8, 8 }, false },
    { { 496, 720, 8, 8 }, false },
    { { 496, 752, 8, 8 }, false },

    { { 80, 272, 8, 8 }, false },
    { { 112, 272, 8, 8 }, false },
    { { 144, 272, 8, 8 }, false },
    { { 240, 272, 8, 8 }, false },
    { { 272, 272, 8, 8 }, false },
    { { 304, 272, 8, 8 }, false },
    { { 368, 272, 8, 8 }, false },
    { { 400, 272, 8, 8 }, false },
    { { 432, 272, 8, 8 }, false },
    { { 528, 272, 8, 8 }, false },
    { { 560, 272, 8, 8 }, false },
    { { 592, 272, 8, 8 }, false },
    { { 624, 272, 8, 8 }, false },
    // Last Column
    { { 624, 80, 8, 8 }, false },
    { { 624, 144, 8, 8 }, false },
    { { 624, 176, 8, 8 }, false },
    { { 624, 208, 8, 8 }, false },
    { { 624, 240, 8, 8 }, false },
    { { 624, 592, 8, 8 }, false },
    { { 624, 624, 8, 8 }, false },
    { { 624, 752, 8, 8 }, false },
    { { 624, 784, 8, 8 }, false },
    { { 624, 816, 8, 8 }, false },

    { { 240, 208, 8, 8 }, false },
    { { 240, 240, 8, 8 }, false },
    { { 432, 208, 8, 8 }, false },
    { { 432, 240, 8, 8 }, false },

    { { 528, 592, 8, 8 }, false },
    { { 560, 592, 8, 8 }, false },
    { { 592, 592, 8, 8 }, false },
    { { 464, 592, 8, 8 }, false },
    { { 432, 592, 8, 8 }, false },
    { { 400, 592, 8, 8 }, false },
    { { 368, 592, 8, 8 }, false },
    { { 304, 592, 8, 8 }, false },
    { { 272, 592, 8, 8 }, false },
    { { 240, 592, 8, 8 }, false },
    { { 208, 592, 8, 8 }, false },
    { { 144, 592, 8, 8 }, false },
    { { 112, 592, 8, 8 }, false },
    { { 80, 592, 8, 8 }, false },
    { { 48, 592, 8, 8 }, false },

    { { 48, 624, 8, 8 }, false },
    { { 304, 624, 8, 8 }, false },
    { { 368, 624, 8, 8 }, false },

    { { 80, 656, 8, 8 }, false },
    { { 112, 656, 8, 8 }, false },
    { { 208, 656, 8, 8 }, false },
    { { 240, 656, 8, 8 }, false },
    { { 272, 656, 8, 8 }, false },
    { { 304, 656, 8, 8 }, false },
    { { 368, 656, 8, 8 }, false },
    { { 400, 656, 8, 8 }, false },
    { { 432, 656, 8, 8 }, false },
    { { 464, 656, 8, 8 }, false },
    { { 560, 656, 8, 8 }, false },
    { { 592, 656, 8, 8 }, false },

    { { 560, 688, 8, 8 }, false },
    { { 432, 688, 8, 8 }, false },
    { { 240, 688, 8, 8 }, false },
    { { 112, 688, 8, 8 }, false },

    { { 112, 720, 8, 8 }, false },
    { { 240, 720, 8, 8 }, false },
    { { 432, 720, 8, 8 }, false },
    { { 560, 720, 8, 8 }, false },

    { { 592, 752, 8, 8 }, false },
    { { 560, 752, 8, 8 }, false },
    { { 528, 752, 8, 8 }, false },
    { { 432, 752, 8, 8 }, false },
    { { 400, 752, 8, 8 }, false },
    { { 368, 752, 8, 8 }, false },
    { { 304, 752, 8, 8 }, false },
    { { 272, 752, 8, 8 }, false },
    { { 240, 752, 8, 8 }, false },
    { { 144, 752, 8, 8 }, false },
    { { 112, 752, 8, 8 }, false },
    { { 80, 752, 8, 8 }, false },
    { { 48, 752, 8, 8 }, false },

    { { 48, 784, 8, 8 }, false },
    { { 304, 784, 8, 8 }, false },
    { { 368, 784, 8, 8 }, false },

    { { 48, 816, 8, 8 }, false },
    { { 80, 816, 8, 8 }, false },
    { { 112, 816, 8, 8 }, false },
    { { 144, 816, 8, 8 }, false },
    { { 176, 816, 8, 8 }, false },
    { { 208, 816, 8, 8 }, false },
    { { 240, 816, 8, 8 }, false },
    { { 272, 816, 8, 8 }, false },
    { { 304, 816, 8, 8 }, false },
    { { 336, 816, 8, 8 }, false },
    { { 368, 816, 8, 8 }, false },
    { { 400, 816, 8, 8 }, false },
    { { 432, 816, 8, 8 }, false },
    { { 464, 816, 8, 8 }, false },
    { { 496, 816, 8, 8 }, false },
    { { 528, 816, 8, 8 }, false },
    { { 560, 816, 8, 8 }, false },
    { { 592, 816, 8, 8 }, false },

};

// Super pacgum placement
BigPellet bigPellets[NUM_BIG_PELLETS] = {
   { { 45, 108, 27, 27 }, false },
   { { 621, 108, 27, 27 }, false },
   { { 45, 650, 27, 27 }, false },
   { { 621, 650, 27, 27 }, false },
};

// DIsplay titles
void display_highscore_title(){
    SDL_Rect highscoreLetters[9] = { letters[7], letters[8], letters[6], letters[7], letters[18],
    letters[2], letters[14], letters[17], letters[4]
    };
    SDL_Rect posInit = { 720, 30, 25, 25 }; // Position initiale de la lettre "H"
    int x = posInit.x;
    for (int i = 0; i < 9; i++) {
        SDL_Rect letterPos = { x, posInit.y, posInit.w, posInit.h };
        SDL_BlitScaled(plancheSprites, &highscoreLetters[i], win_surf, &letterPos);
        x += 30; // Augmenter x de 30 pour la position de la lettre suivante
    }
}

void display_score_title(){
    SDL_Rect lettersScore[5] = { letters[18], letters[2], letters[14], letters[17], letters[4]};
    SDL_Rect posInit = { 720, 200, 25, 25 }; // Position initiale de la lettre "S"
    int x = posInit.x;
    for (int i = 0; i < 5; i++) {
        SDL_Rect letterPos = { x, posInit.y, posInit.w, posInit.h };
        SDL_BlitScaled(plancheSprites, &lettersScore[i], win_surf, &letterPos);
        x += 30; // Augmenter x de 30 pour la position de la lettre suivante
    }
}

// Display scores
void display_game_score() {
    int scoreToDisplay = score;
    int x = 820;
    int y = 250;
    SDL_Rect number[10] = {numbers[0], numbers[1], numbers[2], numbers[3], numbers[4], numbers[5],
    numbers[6], numbers[7], numbers[8], number[9]};
    while (scoreToDisplay > 0) {
        int digit = scoreToDisplay % 10;
        SDL_Rect numberToDisplay = numbers[digit];

        SDL_BlitScaled(plancheSprites, &numberToDisplay, win_surf, &(SDL_Rect){x, y, 25, 25});
        scoreToDisplay /= 10;
        x -= 25;
    }
    if (score == 0) {
        SDL_BlitScaled(plancheSprites, &numbers[0], win_surf, &(SDL_Rect){x, y, 25, 25});
    }
}

void display_game_highscore() {
    int highscoreToDisplay = highscore;
    int x = 820;
    int y = 80;
    SDL_Rect numbers[10] = {
        numbers[0], numbers[1], numbers[2], numbers[3], numbers[4],
        numbers[5], numbers[6], numbers[7], numbers[8], numbers[9]
    };
    while (highscoreToDisplay > 0) {
        int digit = highscoreToDisplay % 10;
        SDL_Rect numberToDisplay = numbers[digit];

        SDL_BlitScaled(plancheSprites, &numberToDisplay, win_surf, &(SDL_Rect){x, y, 25, 25});
        highscoreToDisplay /= 10;
        x -= 25;
    }
    if (highscore == 0) {
        SDL_BlitScaled(plancheSprites, &numbers[0], win_surf, &(SDL_Rect){x, y, 25, 25});
    }
}

// Display lifes remaining
void display_lifes(int pacman_lifes_count){
    // set background to black to erase previous lifes
    SDL_FillRect(win_surf, &(SDL_Rect){720, 400, 150, 50}, SDL_MapRGB(win_surf->format, 0, 0, 0));
    SDL_Rect pacmanLifePos = { 720, 400, 50, 50 };
    for (int i = 0; i < pacman_lifes_count; i++) {
        SDL_BlitScaled(plancheSprites, &pacman_right, win_surf, &pacmanLifePos);
        pacmanLifePos.x += 50;
    }
}

// Display all pacgums
void draw_all_pellets(){
    // Draw the pellet if it hasn't been eaten
    for (int i = 0; i < NUM_PELLETS; i++)
    {
        if (!pellets[i].eaten)
        {
            SDL_FillRect(win_surf, &pellets[i].rect, SDL_MapRGB(win_surf->format, 252, 188, 176));
        }
    }
    // Draw the big pellet if it hasn't been eaten
    for (int i = 0; i < NUM_BIG_PELLETS; i++)
    {
        if (!bigPellets[i].eaten)
        {
            SDL_BlitScaled(plancheSprites, &bigPellet, win_surf, &bigPellets[i].rect);
        }
    }
}

// Respawn pacman and ghosts
void respawn(){

    // Reset the position of pacman and the ghost
    ghost_red.rect = (SDL_Rect){ 325, 325, 32, 32 };
    ghost_pink.rect = (SDL_Rect){ 325, 400, 32, 32 };
    ghost_cyan.rect = (SDL_Rect){ 325, 425, 32, 32 };
    ghost_orange.rect = (SDL_Rect){ 325, 400, 32, 32 };

    pacman = (SDL_Rect){ 325, 646, 32, 32 }; // Position initiale de Pacman
    lastDirection = -1;

    ghost_red.direction = 0;
    ghost_pink.direction = 1;
    ghost_cyan.direction = 1;
    ghost_orange.direction = 1;

    SDL_Delay(1000);
}

// Animate pacman death
void animatePacmanDeath(SDL_Surface* win_surf, SDL_Surface* plancheSprites, int numFrames, SDL_Rect PacmanPos, Uint32 animationDelay) {
    Uint32 lastAnimationChangeTime = SDL_GetTicks();
    int currentFrame = 0;

    // Sprites into table
    SDL_Rect pacmanDeath[10] = { pacman_death1, pacman_death2, pacman_death3, pacman_death4, pacman_death5, pacman_death6, pacman_death7, pacman_death8, pacman_death9, pacman_death10};

    while (currentFrame < numFrames) {
        Uint32 currentTime = SDL_GetTicks();

        if (currentTime - lastAnimationChangeTime >= animationDelay) {
            lastAnimationChangeTime = currentTime;
            currentFrame++;

            SDL_FillRect(win_surf, &PacmanPos, SDL_MapRGB(win_surf->format, 0, 0, 0));
            SDL_BlitScaled(plancheSprites, &pacmanDeath[currentFrame], win_surf, &PacmanPos);
            SDL_UpdateWindowSurface(pWindow);
        }
    }
}

void CheckCollisionWithGhost(SDL_Rect pacman) {
    // Check collision with each ghost individually
    if (SDL_HasIntersection(&pacman, (SDL_Rect*)&ghost_red) == SDL_TRUE ||
        SDL_HasIntersection(&pacman, (SDL_Rect*)&ghost_pink) == SDL_TRUE ||
        SDL_HasIntersection(&pacman, (SDL_Rect*)&ghost_cyan) == SDL_TRUE ||
        SDL_HasIntersection(&pacman, (SDL_Rect*)&ghost_orange) == SDL_TRUE) {

        lifes--;
        if (lifes < 1) {
            gameLost = true;
        }
        else {
            animatePacmanDeath(win_surf, plancheSprites, 10, pacman, 120);
            respawn();
        }
    }
}

bool checkWallCollision(SDL_Rect rect)
{
    // Check if the given rectangle collides with any of the walls
    for (int i = 0; i < NUM_WALLS; i++)
    {
        if (SDL_HasIntersection(&rect, &walls[i]) == SDL_TRUE)
        {
            // Collision detected with wall
            return true;
        }
    }

    // No collision with walls detected
    return false;
}

bool checkPelletCollision(SDL_Rect rect)
{
    // Check collision with the pellet
    for (int i = 0; i < NUM_PELLETS; i++)
    {
        if (!pellets[i].eaten && SDL_HasIntersection(&rect, &pellets[i].rect) == SDL_TRUE)
        {
            // Pellet has been eaten
            pellets[i].eaten = true;
            isPelletEaten = true;
            score += 10;  // Increase the score by 10
        }
    }

    // Check collision with the big pellet
    for (int i = 0; i < NUM_BIG_PELLETS; i++)
    {
        if (!bigPellets[i].eaten && SDL_HasIntersection(&rect, &bigPellets[i].rect) == SDL_TRUE)
        {
            // Big pellet has been eaten
            bigPellets[i].eaten = true;
            isBigPelletEaten = true;
            score += 50;  // Increase the score by 50
        }
    }


    bool allPelletsEaten = true;
    for (int i = 0; i < NUM_PELLETS; i++)
    {
        if (!pellets[i].eaten)
        {
            allPelletsEaten = false;
            break;
        }
    }

    bool allBigPelletsEaten = true;
    for (int i = 0; i < NUM_BIG_PELLETS; i++)
    {
        if (!bigPellets[i].eaten)
        {
            allBigPelletsEaten = false;
            break;
        }
    }

    // Set gameWon flag if all pellets are eaten
    if (allPelletsEaten && allBigPelletsEaten)
    {
        gameWon = true;
    }

    // No collision with pellets detected
    return false;
}

bool checkCollision(SDL_Rect rect)
{
    bool wallCollision = checkWallCollision(rect);
    bool pelletCollision = checkPelletCollision(rect);

    // Collision detected if there is a wall or pellet collision
    return wallCollision || pelletCollision;
}

// Initialize SDL
void init()
{
    pWindow = SDL_CreateWindow("PacMan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 900, SDL_WINDOW_SHOWN);
    win_surf = SDL_GetWindowSurface(pWindow);

    plancheSprites = SDL_LoadBMP("./pacman_sprites.bmp");

      // Get the path of the current source file
    char currentPath[FILENAME_MAX];
    if (realpath(__FILE__, currentPath) == NULL)
    {
        fprintf(stderr, "Failed to get current file path.\n");
        // Handle the error accordingly
    }

    // Get the directory path of the current source file
    char* fontDirectory = dirname(currentPath);

    // Set the working directory to the directory where the font file is located
    if (chdir(fontDirectory) != 0)
    {
        fprintf(stderr, "Failed to set working directory to font directory.\n");
        // Handle the error accordingly
    }
}

// Ghosts movement
void moveGhosts()
{
    struct GhostData
    {
        SDL_Rect* position;
        int* direction;
        SDL_Rect* sprites[4];
        int* count;
    };

    struct GhostData ghostsData[] = {
        { &ghost_red.rect, &ghost_red.direction, { &ghost_red.sprites[0], &ghost_red.sprites[1], &ghost_red.sprites[2], &ghost_red.sprites[3] }, &ghost_red.count },
        { &ghost_pink.rect, &ghost_pink.direction, { &ghost_pink.sprites[0], &ghost_pink.sprites[1], &ghost_pink.sprites[2], &ghost_pink.sprites[3] }, &ghost_pink.count },
        { &ghost_cyan.rect, &ghost_cyan.direction, { &ghost_cyan.sprites[0], &ghost_cyan.sprites[1], &ghost_cyan.sprites[2], &ghost_cyan.sprites[3] }, &ghost_cyan.count },
        { &ghost_orange.rect, &ghost_orange.direction, { &ghost_orange.sprites[0], &ghost_orange.sprites[1], &ghost_orange.sprites[2], &ghost_orange.sprites[3] }, &ghost_orange.count }
    };

    for (int i = 0; i < sizeof(ghostsData) / sizeof(ghostsData[0]); i++)
    {
        struct GhostData* ghostData = &ghostsData[i];
        SDL_Rect* ghostPosition = ghostData->position;
        int* ghostDirection = ghostData->direction;
        SDL_Rect** ghostSprites = ghostData->sprites;
        int* ghostCount = ghostData->count;

        int prevX = ghostPosition->x;
        int prevY = ghostPosition->y;
        int prevDirection = *ghostDirection;

        // Update the ghost's position based on the current direction
        switch (*ghostDirection)
        {
        case 0: // Move left
            ghostPosition->x--;
            break;
        case 1: // Move up
            ghostPosition->y--;
            break;
        case 2: // Move right
            ghostPosition->x++;
            break;
        case 3: // Move down
            ghostPosition->y++;
            break;
        }

        // Check for wall collision
        if (checkWallCollision(*ghostPosition))
        {
            // Revert the ghost's movement
            ghostPosition->x = prevX;
            ghostPosition->y = prevY;

            // Generate a random valid direction
            int newDirection;
            do
            {
                newDirection = rand() % 4;
            } while (newDirection == prevDirection || newDirection == (prevDirection + 2) % 4);

            // Update the direction
            *ghostDirection = newDirection;
        }

        // Update the count for animation
        *ghostCount = (*ghostCount + 1) % 512;

        // Determine the sprite direction based on the current direction
        SDL_Rect* currentSprite = ghostSprites[*ghostDirection];
        SDL_Rect ghostInSprite = *currentSprite;
        if ((*ghostCount / 4) % 2)
            ghostInSprite.x += 17;

        SDL_SetColorKey(plancheSprites, true, 0);
        SDL_BlitScaled(plancheSprites, &ghostInSprite, win_surf, ghostPosition);
    }
}

void updatePacman()
{
        // Check if pacman enter tunnel, teleport him to the other side
    if (pacman.x < 10)
    {
        pacman.x = 645;
    }
    else if (pacman.x > 645)
    {
        pacman.x = 10;
    }


    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastAnimationChangeTime >= animationDelay)
    {
        // Toggle the animation state between wide open and normal
        isWideOpen = !isWideOpen;
        lastAnimationChangeTime = currentTime; // Update the last animation change time
    }

    // Determine the appropriate sprite based on the animation state and direction
    SDL_Rect pacman_direction;
    if (lastDirection == SDL_SCANCODE_LEFT)
    {
        if (isWideOpen)
        {
            pacman_direction = pacman_wide_left;
        }
        else
        {
            pacman_direction = pacman_left;
        }
    }
    else if (lastDirection == SDL_SCANCODE_RIGHT)
    {
        if (isWideOpen)
        {
            pacman_direction = pacman_wide_right;
        }
        else
        {
            pacman_direction = pacman_right;
        }
    }
    else if (lastDirection == SDL_SCANCODE_UP)
    {
        if (isWideOpen)
        {
            pacman_direction = pacman_wide_up;
        }
        else
        {
            pacman_direction = pacman_up;
        }
    }
    else if (lastDirection == SDL_SCANCODE_DOWN)
    {
        if (isWideOpen)
        {
            pacman_direction = pacman_wide_down;
        }
        else
        {
            pacman_direction = pacman_down;
        }
    }
    else
    {
        pacman_direction = pacman_closed;
        isWideOpen = false;  // Reset animation state when not moving left
    }

    SDL_Rect nextPosition = pacman;
    switch (lastDirection)
    {
    case SDL_SCANCODE_LEFT:
        nextPosition.x--;
        break;
    case SDL_SCANCODE_RIGHT:
        nextPosition.x++;
        break;
    case SDL_SCANCODE_UP:
        nextPosition.y--;
        break;
    case SDL_SCANCODE_DOWN:
        nextPosition.y++;
        break;
    }

    SDL_BlitScaled(plancheSprites, &pacman_direction, win_surf, &pacman);

    // Check collision with solid walls before updating Pacman's position
    if (!checkCollision(nextPosition))
    {
        pacman = nextPosition;
    }
}

void draw()
{
    SDL_SetColorKey(plancheSprites, false, 0);
    SDL_BlitScaled(plancheSprites, &src_bg, win_surf, &bg);

    display_highscore_title();
    display_score_title();
    display_game_score();
    display_game_highscore();
    display_lifes(lifes);

    draw_all_pellets();

        // Check collision with ghost and update lifes
    CheckCollisionWithGhost(pacman);
    moveGhosts();
    updatePacman();
}

// Draw the start menu
void drawStartMenu(SDL_Surface* surface) {

    // Remplir la surface avec une couleur de fond noire
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));

    // Afficher le sprite welcome_title au centre en haut
    SDL_Rect welcomeTitlePos = { surface->w / 2 - 270, surface->h / 2 - 300, 540, 138 };
    SDL_BlitScaled(plancheSprites, &welcome_title, surface, &welcomeTitlePos);


    // Afficher les choix "Play -P" et "Quit -Q"
    SDL_Rect playPos = { surface->w / 2 - ((30*7)/2), surface->h / 2 - 50, 25, 25 };
    SDL_Rect playLetters[7] = { letters[15], letters[11], letters[0], letters[24], espace, tiret, letters[15] };
    for (int i = 0; i < 7; i++) {
        SDL_BlitScaled(plancheSprites, &playLetters[i], surface, &playPos);
        playPos.x += 30;
    }

    SDL_Rect quitPos = { surface->w / 2 - ((30*7)/2), surface->h / 2 - 0, 25, 25 };
    SDL_Rect quitLetters[7] = { letters[16], letters[20], letters[8], letters[19], espace, tiret, letters[16] };
    for (int i = 0; i < 7; i++) {
        SDL_BlitScaled(plancheSprites, &quitLetters[i], surface, &quitPos);
        quitPos.x += 30;
    }

    // Display all 4 ghosts sprites at the bottom of the screen
    SDL_Rect ghost_red_pos = { surface->w / 2 - 160, surface->h / 2 + 100, 32, 32 };
    SDL_BlitScaled(plancheSprites, &ghost_red.sprites[ghost_red.direction], surface, &ghost_red_pos);

    SDL_Rect ghost_pink_pos = { surface->w / 2 - 60, surface->h / 2 + 100, 32, 32 };
    SDL_BlitScaled(plancheSprites, &ghost_pink.sprites[ghost_pink.direction], surface, &ghost_pink_pos);

    SDL_Rect ghost_cyan_pos = { surface->w / 2 + 40, surface->h / 2 + 100, 32, 32 };
    SDL_BlitScaled(plancheSprites, &ghost_cyan.sprites[ghost_cyan.direction], surface, &ghost_cyan_pos);

    SDL_Rect ghost_orange_pos = { surface->w / 2 + 140, surface->h / 2 + 100, 32, 32 };
    SDL_BlitScaled(plancheSprites, &ghost_orange.sprites[ghost_orange.direction], surface, &ghost_orange_pos);
}
// Draw the end menu
void drawEndMenu(SDL_Surface* surface, bool gameLost) {
    // Remplir la surface avec une couleur de fond noire
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));

    // Afficher le message "You Win!" ou "You Lost!"
    if (!gameLost) {
        SDL_Rect youWinPos = { surface->w / 2 - 120, surface->h / 2 - 200, 25, 25 };
        SDL_Rect youWinLetters[9] = { letters[24], letters[14], letters[20], espace, letters[22], letters[8], letters[13], espace, exclamation };
        for (int i = 0; i < 9; i++) {
            SDL_BlitScaled(plancheSprites, &youWinLetters[i], surface, &youWinPos);
            youWinPos.x += 30;
        }
    } else {
        SDL_Rect youLostPos = { surface->w / 2 - 120, surface->h / 2 - 200, 25, 25 };
        SDL_Rect youLostLetters[10] = { letters[24], letters[14], letters[20], espace, letters[11], letters[14], letters[18], letters[19], espace, exclamation };
        for (int i = 0; i < 10; i++) {
            SDL_BlitScaled(plancheSprites, &youLostLetters[i], surface, &youLostPos);
            youLostPos.x += 30;
        }
    }

    // Afficher les choix "Try again - T", Return to menu - R" et "Quit - Q"
    SDL_Rect tryAgainPos = { surface->w / 2 - 170, surface->h / 2 - 50, 25, 25 };
    SDL_Rect tryAgainLetters[12] = { letters[19], letters[17], letters[24], espace, letters[0], letters[6], letters[0], letters[8], letters[13], espace, tiret, letters[19]};
    for (int i = 0; i < 12; i++) {
        SDL_BlitScaled(plancheSprites, &tryAgainLetters[i], surface, &tryAgainPos);
        tryAgainPos.x += 30;
    }

    SDL_Rect returnToMenuPos = { surface->w / 2 - 245, surface->h / 2 - 0, 25, 25 };
    SDL_Rect returnToMenuLetters[18] = { letters[17], letters[4], letters[19], letters[20], letters[17],
        letters[13], espace, letters[19], letters[14], espace, letters[12], letters[4], letters[13],
        letters[20], espace, tiret, letters[17]
    };

    for (int i = 0; i < 18; i++) {
        SDL_BlitScaled(plancheSprites, &returnToMenuLetters[i], surface, &returnToMenuPos);
        returnToMenuPos.x += 30;
    }

    SDL_Rect quitPos = { surface->w / 2 - 100, surface->h / 2 + 50, 25, 25 };
    SDL_Rect quitLetters[7] = { letters[16], letters[20], letters[8], letters[19], espace, tiret, letters[16] };
    for (int i = 0; i < 7; i++) {
        SDL_BlitScaled(plancheSprites, &quitLetters[i], surface, &quitPos);
        quitPos.x += 30;
    }
}

void handleInput(SDL_Event* event)
{
    switch (event->type)
    {
    case SDL_QUIT:
        exit(0);
        break;
    case SDL_KEYDOWN:
        switch (event->key.keysym.sym)
        {
        case SDLK_LEFT:
            lastDirection = SDL_SCANCODE_LEFT;
            break;
        case SDLK_RIGHT:
            lastDirection = SDL_SCANCODE_RIGHT;
            break;
        case SDLK_UP:
            lastDirection = SDL_SCANCODE_UP;
            break;
        case SDLK_DOWN:
            lastDirection = SDL_SCANCODE_DOWN;
            break;
        }
        break;
    }
}

void restartGame()
{
    // Reset the game state and variables
    isPelletEaten = false;
    bool isBigPelletEaten = false;
    gameLost = false;
    gameWon = false;
    if (score > highscore) {
        highscore = score;
    }
    score = 0;
    lifes = 3;
    lastDirection = -1; // Reset the last key pressed direction
    pacman = (SDL_Rect){332, 642, 32, 32}; // Reset Pacman's position

    // Reset the ghosts' positions
    ghost_red.rect = (SDL_Rect){ 325, 325, 32, 32 };
    ghost_pink.rect = (SDL_Rect){ 325, 400, 32, 32 };
    ghost_cyan.rect = (SDL_Rect){ 325, 425, 32, 32 };
    ghost_orange.rect = (SDL_Rect){ 325, 400, 32, 32 };

    // Reset the ghosts' directions
    ghost_red.direction = 0;
    ghost_pink.direction = 1;
    ghost_cyan.direction = 1;
    ghost_orange.direction = 1;

    // Reset the eaten state of all pellets
    for (int i = 0; i < NUM_PELLETS; i++)
    {
        pellets[i].eaten = false;
    }
    for (int i = 0; i < NUM_BIG_PELLETS; i++)
    {
        bigPellets[i].eaten = false;
    }
}

void gameLoop()
{
    bool quit = false;
    bool restart = false;
    bool startMenu = true;

    while (!quit)
    {
        // Display the start menu
        while (startMenu)
        {
            drawStartMenu(win_surf);
            SDL_UpdateWindowSurface(pWindow);
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
             if (event.type == SDL_QUIT)
                {
                    quit = true;
                    startMenu = false;
                }
                else if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.scancode == SDL_SCANCODE_P)
                    {
                        startMenu = false;
                        // black background to erase menu before starting game
                        SDL_FillRect(win_surf, NULL, SDL_MapRGB(win_surf->format, 0, 0, 0));

                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_A)
                    {
                        quit = true;
                        startMenu = false;
                    }
                }
            }

        }

        // Game loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    quit = true;
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT ||
                         event.key.keysym.scancode == SDL_SCANCODE_RIGHT ||
                         event.key.keysym.scancode == SDL_SCANCODE_UP ||
                         event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                {
                    lastDirection = event.key.keysym.scancode;
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_T)
                {
                    if (gameWon)
                    {
                        restartGame();
                    }
                    restart = true;
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_A)
                {
                    quit = true;
                    printf("Quitting the game...\n");
                }
            }
        }

        if (restart)
        {
            restart = false;
        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_ESCAPE])
        {
            quit = true;
        }

        draw();
        SDL_UpdateWindowSurface(pWindow);

        // Display the end menu if the game is won or lost
        if (gameWon || gameLost) {

            SDL_FillRect(win_surf, NULL, SDL_MapRGB(win_surf->format, 0, 0, 0));
            drawEndMenu(win_surf, gameLost); //if gameLost = true, display You Lost, if gameLost = false, display You Win
            SDL_UpdateWindowSurface(pWindow);

            bool menuActive = true;
            while (menuActive)
            {
                SDL_Event menuEvent;
                while (SDL_PollEvent(&menuEvent))
                {
                    if (menuEvent.type == SDL_QUIT)
                    {
                        quit = true;
                        menuActive = false;
                    }
                    else if (menuEvent.type == SDL_KEYDOWN)
                    {
                        if (menuEvent.key.keysym.scancode == SDL_SCANCODE_T)
                        {
                            SDL_FillRect(win_surf, NULL, SDL_MapRGB(win_surf->format, 0, 0, 0));
                            restartGame(); // Restart the game
                            menuActive = false;
                        }
                        else if (menuEvent.key.keysym.scancode == SDL_SCANCODE_R)
                        {
                            menuActive = false;
                            restartGame(); // Restart the game in case the player will play again after going through the start menu
                            startMenu = true; // Display the start menu

                        }
                        else if (menuEvent.key.keysym.scancode == SDL_SCANCODE_A)
                        {
                            SDL_FillRect(win_surf, NULL, SDL_MapRGB(win_surf->format, 0, 0, 0));
                            menuActive = false;
                            startMenu = true; // Display the start menu
                        }
                    }
                }
            }
        }

        SDL_Delay(1); // Delay to control the game loop speed
    }
}

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