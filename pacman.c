#include <SDL.h>
#include <SDL_ttf.h>
#include <unistd.h>
#include <libgen.h> // For dirname function
#include <stdio.h>
#include <stdbool.h>

SDL_Window* pWindow = NULL;
SDL_Surface* win_surf = NULL;
SDL_Surface* plancheSprites = NULL;
TTF_Font* font = NULL;

SDL_Rect src_bg = { 369, 3, 168, 216 };
SDL_Rect bg = { 4, 4, 672, 864 };

SDL_Rect ghost_r = { 3, 123, 16, 16 };
SDL_Rect ghost_l = { 37, 123, 16, 16 };
SDL_Rect ghost_d = { 105, 123, 16, 16 };
SDL_Rect ghost_u = { 71, 123, 16, 16 };
SDL_Rect ghost = { 34, 34, 32, 32 };

SDL_Rect pacman_closed = { 3, 90, 16, 16 };
SDL_Rect pacman_right = { 20, 90, 16, 16 };
SDL_Rect pacman_left = { 46, 90, 16, 16 };
SDL_Rect pacman_up = { 75, 90, 16, 16 };
SDL_Rect pacman_down = { 109, 90, 16, 16 };

SDL_Rect bigPellet = { 9, 79, 8, 8 };

bool isPelletEaten = false;
bool isBigPelletEaten = false;
bool gameWon = false;

int count;

SDL_Rect pacman = { 340 - (16 / 2), 650 - (16 / 2), 32, 32 };
int lastDirection = -1; // Store the last key pressed direction

#define NUM_WALLS 47
#define NUM_PELLETS 1
#define NUM_BIG_PELLETS 1

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

typedef struct {
    SDL_Rect rect;
    bool eaten;
} Pellet;

Pellet pellets[NUM_PELLETS] = {
    // { { 176, 48, 8, 8 }, false },
    // { { 176, 80, 8, 8 }, false },
    // { { 176, 112, 8, 8 }, false },
    // { { 176, 144, 8, 8 }, false },
    // { { 176, 176, 8, 8 }, false },
    // { { 176, 208, 8, 8 }, false },
    // { { 176, 240, 8, 8 }, false },
    // { { 176, 272, 8, 8 }, false },
    // { { 176, 304, 8, 8 }, false },
    // { { 176, 336, 8, 8 }, false },
    // { { 176, 368, 8, 8 }, false },
    // { { 176, 400, 8, 8 }, false },
    // { { 176, 432, 8, 8 }, false },
    // { { 176, 464, 8, 8 }, false },
    // { { 176, 496, 8, 8 }, false },
    // { { 176, 528, 8, 8 }, false },
    // { { 176, 560, 8, 8 }, false },
    // { { 176, 592, 8, 8 }, false },
    // { { 176, 624, 8, 8 }, false },
    // { { 176, 656, 8, 8 }, false },
    // { { 176, 688, 8, 8 }, false },
    // { { 176, 720, 8, 8 }, false },
    // { { 176, 752, 8, 8 }, false },

    // { { 48, 176, 8, 8 }, false },
    // { { 80, 176, 8, 8 }, false },
    // { { 112, 176, 8, 8 }, false },
    // { { 144, 176, 8, 8 }, false },
    // { { 176, 176, 8, 8 }, false },
    // { { 208, 176, 8, 8 }, false },
    // { { 240, 176, 8, 8 }, false },
    // { { 272, 176, 8, 8 }, false },
    // { { 304, 176, 8, 8 }, false },
    // { { 336, 176, 8, 8 }, false },
    // { { 368, 176, 8, 8 }, false },
    // { { 400, 176, 8, 8 }, false },
    // { { 432, 176, 8, 8 }, false },
    // { { 464, 176, 8, 8 }, false },
    // { { 496, 176, 8, 8 }, false },
    // { { 528, 176, 8, 8 }, false },
    // { { 560, 176, 8, 8 }, false },
    // { { 592, 176, 8, 8 }, false },
    // { { 624, 176, 8, 8 }, false },

    // { { 48, 48, 8, 8 }, false },
    // { { 48, 80, 8, 8 }, false },
    // { { 48, 144, 8, 8 }, false },
    // { { 48, 176, 8, 8 }, false },
    // { { 48, 208, 8, 8 }, false },
    // { { 48, 240, 8, 8 }, false },
    // { { 48, 272, 8, 8 }, false },

    // { { 80, 48, 8, 8 }, false },
    // { { 112, 48, 8, 8 }, false },
    // { { 144, 48, 8, 8 }, false },
    // { { 208, 48, 8, 8 }, false },
    // { { 240, 48, 8, 8 }, false },
    // { { 272, 48, 8, 8 }, false },
    // { { 304, 48, 8, 8 }, false },

    // { { 304, 80, 8, 8 }, false },
    // { { 304, 112, 8, 8 }, false },
    // { { 304, 144, 8, 8 }, false },

    // { { 368, 48, 8, 8 }, false },
    // { { 400, 48, 8, 8 }, false },
    // { { 432, 48, 8, 8 }, false },
    // { { 464, 48, 8, 8 }, false },
    // { { 496, 48, 8, 8 }, false },
    // { { 528, 48, 8, 8 }, false },
    // { { 560, 48, 8, 8 }, false },
    // { { 592, 48, 8, 8 }, false },
    // { { 624, 48, 8, 8 }, false },

    // { { 368, 80, 8, 8 }, false },
    // { { 368, 112, 8, 8 }, false },
    // { { 368, 144, 8, 8 }, false },

    // { { 496, 80, 8, 8 }, false },
    // { { 496, 112, 8, 8 }, false },
    // { { 496, 144, 8, 8 }, false },
    // { { 496, 176, 8, 8 }, false },
    // { { 496, 208, 8, 8 }, false },
    // { { 496, 240, 8, 8 }, false },
    // { { 496, 272, 8, 8 }, false },
    // { { 496, 304, 8, 8 }, false },
    // { { 496, 336, 8, 8 }, false },
    // { { 496, 368, 8, 8 }, false },
    // { { 496, 400, 8, 8 }, false },
    // { { 496, 432, 8, 8 }, false },
    // { { 496, 464, 8, 8 }, false },
    // { { 496, 496, 8, 8 }, false },
    // { { 496, 528, 8, 8 }, false },
    // { { 496, 560, 8, 8 }, false },
    // { { 496, 592, 8, 8 }, false },
    // { { 496, 624, 8, 8 }, false },
    // { { 496, 656, 8, 8 }, false },
    // { { 496, 688, 8, 8 }, false },
    // { { 496, 720, 8, 8 }, false },
    // { { 496, 752, 8, 8 }, false },

    // { { 80, 272, 8, 8 }, false },
    // { { 112, 272, 8, 8 }, false },
    // { { 144, 272, 8, 8 }, false },
    // { { 240, 272, 8, 8 }, false },
    // { { 272, 272, 8, 8 }, false },
    // { { 304, 272, 8, 8 }, false },
    // { { 368, 272, 8, 8 }, false },
    // { { 400, 272, 8, 8 }, false },
    // { { 432, 272, 8, 8 }, false },
    // { { 528, 272, 8, 8 }, false },
    // { { 560, 272, 8, 8 }, false },
    // { { 592, 272, 8, 8 }, false },
    // { { 624, 272, 8, 8 }, false },
    // // Last Column
    // { { 624, 80, 8, 8 }, false },
    // { { 624, 144, 8, 8 }, false },
    // { { 624, 176, 8, 8 }, false },
    // { { 624, 208, 8, 8 }, false },
    // { { 624, 240, 8, 8 }, false },
    // { { 624, 592, 8, 8 }, false },
    // { { 624, 624, 8, 8 }, false },
    // { { 624, 752, 8, 8 }, false },
    // { { 624, 784, 8, 8 }, false },
    // { { 624, 816, 8, 8 }, false },

    // { { 240, 208, 8, 8 }, false },
    // { { 240, 240, 8, 8 }, false },
    // { { 432, 208, 8, 8 }, false },
    // { { 432, 240, 8, 8 }, false },

    // { { 528, 592, 8, 8 }, false },
    // { { 560, 592, 8, 8 }, false },
    // { { 592, 592, 8, 8 }, false },
    // { { 464, 592, 8, 8 }, false },
    // { { 432, 592, 8, 8 }, false },
    // { { 400, 592, 8, 8 }, false },
    // { { 368, 592, 8, 8 }, false },
    // { { 304, 592, 8, 8 }, false },
    // { { 272, 592, 8, 8 }, false },
    // { { 240, 592, 8, 8 }, false },
    // { { 208, 592, 8, 8 }, false },
    // { { 144, 592, 8, 8 }, false },
    // { { 112, 592, 8, 8 }, false },
    // { { 80, 592, 8, 8 }, false },
    // { { 48, 592, 8, 8 }, false },

    // { { 48, 624, 8, 8 }, false },
    // { { 304, 624, 8, 8 }, false },
    // { { 368, 624, 8, 8 }, false },

    // { { 80, 656, 8, 8 }, false },
    // { { 112, 656, 8, 8 }, false },
    // { { 208, 656, 8, 8 }, false },
    // { { 240, 656, 8, 8 }, false },
    // { { 272, 656, 8, 8 }, false },
    // { { 304, 656, 8, 8 }, false },
    // { { 368, 656, 8, 8 }, false },
    // { { 400, 656, 8, 8 }, false },
    // { { 432, 656, 8, 8 }, false },
    // { { 464, 656, 8, 8 }, false },
    // { { 560, 656, 8, 8 }, false },
    // { { 592, 656, 8, 8 }, false },

    // { { 560, 688, 8, 8 }, false },
    // { { 432, 688, 8, 8 }, false },
    // { { 240, 688, 8, 8 }, false },
    // { { 112, 688, 8, 8 }, false },

    // { { 112, 720, 8, 8 }, false },
    // { { 240, 720, 8, 8 }, false },
    // { { 432, 720, 8, 8 }, false },
    // { { 560, 720, 8, 8 }, false },

    // { { 592, 752, 8, 8 }, false },
    // { { 560, 752, 8, 8 }, false },
    // { { 528, 752, 8, 8 }, false },
    // { { 432, 752, 8, 8 }, false },
    // { { 400, 752, 8, 8 }, false },
    // { { 368, 752, 8, 8 }, false },
    // { { 304, 752, 8, 8 }, false },
    // { { 272, 752, 8, 8 }, false },
    // { { 240, 752, 8, 8 }, false },
    // { { 144, 752, 8, 8 }, false },
    // { { 112, 752, 8, 8 }, false },
    // { { 80, 752, 8, 8 }, false },
    // { { 48, 752, 8, 8 }, false },

    // { { 48, 784, 8, 8 }, false },
    // { { 304, 784, 8, 8 }, false },
    // { { 368, 784, 8, 8 }, false },

    // { { 48, 816, 8, 8 }, false },
    // { { 80, 816, 8, 8 }, false },
    // { { 112, 816, 8, 8 }, false },
    // { { 144, 816, 8, 8 }, false },
    // { { 176, 816, 8, 8 }, false },
    // { { 208, 816, 8, 8 }, false },
    // { { 240, 816, 8, 8 }, false },
    // { { 272, 816, 8, 8 }, false },
    // { { 304, 816, 8, 8 }, false },
    // { { 336, 816, 8, 8 }, false },
    // { { 368, 816, 8, 8 }, false },
    // { { 400, 816, 8, 8 }, false },
    // { { 432, 816, 8, 8 }, false },
    // { { 464, 816, 8, 8 }, false },
    // { { 496, 816, 8, 8 }, false },
    // { { 528, 816, 8, 8 }, false },
    // { { 560, 816, 8, 8 }, false },
    { { 592, 816, 8, 8 }, false },

};

typedef struct {
   SDL_Rect rect;
   bool eaten;
} BigPellet;

// Placement des big pellets
BigPellet bigPellets[NUM_BIG_PELLETS] = {
   { { 45, 108, 16, 16 }, false },
//    { { 621, 108, 16, 16 }, false },
//    { { 45, 650, 16, 16 }, false },
//    { { 621, 650, 16, 16 }, false },
};

bool checkCollision(SDL_Rect rect)
{
    // Check if the given rectangle collides with any of the walls
    for (int i = 0; i < NUM_WALLS; i++)
    {
        if (SDL_HasIntersection(&rect, &walls[i]) == SDL_TRUE)
        {
            // Collision detected
            return true;
        }
    }

    // Check collision with the pellet
    for (int i = 0; i < NUM_PELLETS; i++)
    {
        if (!pellets[i].eaten && SDL_HasIntersection(&rect, &pellets[i].rect) == SDL_TRUE)
        {
            // Pellet has been eaten
            pellets[i].eaten = true;
            isPelletEaten = true;
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

    // No collision detected
    return false;
}

void init()
{
    pWindow = SDL_CreateWindow("PacMan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 700, 900, SDL_WINDOW_SHOWN);
    win_surf = SDL_GetWindowSurface(pWindow);

    plancheSprites = SDL_LoadBMP("./pacman_sprites.bmp");
    count = 0;

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

    // Initialize SDL_ttf
    TTF_Init();
    font = TTF_OpenFont("pacman.ttf", 32);
        if (font == NULL)
    {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        // Handle the error accordingly
    }
}

void draw()
{
    SDL_SetColorKey(plancheSprites, false, 0);
    SDL_BlitScaled(plancheSprites, &src_bg, win_surf, &bg);

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

    SDL_Rect* ghost_in = NULL;
    switch (count / 128)
    {
    case 0:
        ghost_in = &ghost_r;
        ghost.x++;
        break;
    case 1:
        ghost_in = &ghost_d;
        ghost.y++;
        break;
    case 2:
        ghost_in = &ghost_l;
        ghost.x--;
        break;
    case 3:
        ghost_in = &ghost_u;
        ghost.y--;
        break;
    }
    count = (count + 1) % 512;

    SDL_Rect ghost_in2 = *ghost_in;
    if ((count / 4) % 2)
        ghost_in2.x += 17;

    SDL_SetColorKey(plancheSprites, true, 0);
    SDL_BlitScaled(plancheSprites, &ghost_in2, win_surf, &ghost);

    SDL_Rect pacman_direction;
    if (lastDirection == SDL_SCANCODE_LEFT)
    {
        pacman_direction = pacman_left;
    }
    else if (lastDirection == SDL_SCANCODE_RIGHT)
    {
        pacman_direction = pacman_right;
    }
    else if (lastDirection == SDL_SCANCODE_UP)
    {
        pacman_direction = pacman_up;
    }
    else if (lastDirection == SDL_SCANCODE_DOWN)
    {
        pacman_direction = pacman_down;
    }
    else
    {
        pacman_direction = pacman_closed;
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
    // Check collision with solid walls before updating Pacman's position
    if (!checkCollision(nextPosition))
    {
        pacman = nextPosition;
    }

    SDL_BlitScaled(plancheSprites, &pacman_direction, win_surf, &pacman);

    // Check if pacman enter tunnel, teleport him to the other side
    if (pacman.x < 10)
    {
        pacman.x = 645;
    }
    else if (pacman.x > 645)
    {
        pacman.x = 10;
    }

}

void drawMenu(SDL_Surface* surface, bool isWinMenu)
{
    // Clear the surface
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));

    // Load the font
    TTF_Font* font = TTF_OpenFont("pacman.ttf", 32);
    if (font == NULL)
    {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        // Handle the error accordingly
    }

    // Create a surface for the menu options
    SDL_Surface* textSurfaceRetry = TTF_RenderText_Solid(font, "Retry (R)", (SDL_Color){255, 255, 255});
    SDL_Surface* textSurfaceQuit = TTF_RenderText_Solid(font, "Quit (Q)", (SDL_Color){255, 255, 255});
    if (textSurfaceRetry == NULL || textSurfaceQuit == NULL)
    {
        fprintf(stderr, "Failed to render text surface: %s\n", TTF_GetError());
        // Handle the error accordingly
    }

    // Calculate the position to display the menu options on the screen
    int textX = (surface->w - textSurfaceRetry->w) / 2;
    int textY = (surface->h - textSurfaceRetry->h) / 2;

    // Blit the menu options onto the surface
    SDL_BlitSurface(textSurfaceRetry, NULL, surface, &(SDL_Rect){textX, textY, 0, 0});
    SDL_BlitSurface(textSurfaceQuit, NULL, surface, &(SDL_Rect){textX, textY + textSurfaceRetry->h + 10, 0, 0});

    // If it's the win menu, render the win message on top
    if (isWinMenu)
    {
        SDL_Surface* winMessage = TTF_RenderText_Solid(font, "You Win!", (SDL_Color){255, 255, 255});
        if (winMessage == NULL)
        {
            fprintf(stderr, "Failed to render win message surface: %s\n", TTF_GetError());
            // Handle the error accordingly
        }

        int messageX = (surface->w - winMessage->w) / 2;
        int messageY = (surface->h - winMessage->h) / 4;
        SDL_BlitSurface(winMessage, NULL, surface, &(SDL_Rect){messageX, messageY, 0, 0});

        SDL_FreeSurface(winMessage);
    }

    // Free the text surfaces
    SDL_FreeSurface(textSurfaceRetry);
    SDL_FreeSurface(textSurfaceQuit);

    // Close the font
    TTF_CloseFont(font);
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
    gameWon = false;
    lastDirection = -1; // Reset the last key pressed direction
    pacman = (SDL_Rect){340 - (16 / 2), 650 - (16 / 2), 32, 32}; // Reset Pacman's position

    // Reset the eaten state of all pellets
    for (int i = 0; i < NUM_PELLETS; i++)
    {
        pellets[i].eaten = false;
    }
}

void gameLoop()
{
    bool quit = false;
    bool restart = false;

    while (!quit)
    {
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
                else if (event.key.keysym.scancode == SDL_SCANCODE_R)
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

        if (gameWon)
        {
            // Draw the win menu
            SDL_FillRect(win_surf, NULL, SDL_MapRGB(win_surf->format, 0, 0, 0));
            drawMenu(win_surf, true);
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
                        if (menuEvent.key.keysym.scancode == SDL_SCANCODE_R)
                        {
                            restartGame();
                            menuActive = false;
                        }
                        else if (menuEvent.key.keysym.scancode == SDL_SCANCODE_A)
                        {
                            quit = true;
                            printf("Quitting the game...\n");
                            menuActive = false;
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL initialization failed: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0)
    {
        fprintf(stderr, "TTF initialization failed: %s", TTF_GetError());
        return 1;
    }

    init();

    gameLoop();

    SDL_DestroyWindow(pWindow);
    SDL_Quit();

    return 0;
}