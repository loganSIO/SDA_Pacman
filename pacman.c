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

bool isPelletEaten = false;
bool gameWon = false;

int count;

SDL_Rect pacman = { 340 - (16 / 2), 650 - (16 / 2), 32, 32 };
int lastDirection = -1; // Store the last key pressed direction

#define NUM_WALLS 43
#define NUM_PELLETS 1

SDL_Rect walls[NUM_WALLS] = {
    // { x, y, w, h }
    { 0, 0, 1680, 34 }, // Top wall
    { 0, 0, 34, 7200 }, // Left side wall
    { 0, 838, 1680, 34 }, // Bottom wall
    { 644, 0, 34, 1680 }, // Bottom wall
    // Left side of the wall
    { 22, 292, 142, 126 }, // first rectangle
    { 22, 455, 142, 128 }, // second rectangle
    { 22, 678, 78, 62 }, // third rectangle
    // Block & line left side
    { 66, 70, 92, 92 }, // first block
    { 66, 202, 92, 50 }, // second rectangle block
    { 198, 70, 92, 92 }, // second block
    { 198, 202, 26, 218 }, // first y line
    { 240, 296, 46, 30 }, // first x line
    { 198, 462, 26, 112 }, // second y line
    { 198, 620, 92, 22 }, // second x line
    { 74, 620, 88, 24 }, // first inversed L x axis
    { 140, 640, 22, 92 }, // first inversed L y axis
    { 198, 680, 26, 120 }, // second y line
    { 74, 780, 212, 24 }, // second y line
    // Mirrored lines on the right side
    { 680 - 22 - 138, 292, 138, 126 }, // First rectangle
    { 680 - 22 - 138, 455, 138, 128 }, // Second rectangle
    { 680 - 22 - 74, 678, 74, 62 },     // Third rectangle
    { 680 - 66 - 78, 70, 70, 92 },      // First block
    { 680 - 66 - 78, 202, 70, 50 },     // Second rectangle block
    { 680 - 198 - 92, 70, 92, 92 },     // Second block
    { 680 - 198 - 26, 202, 26, 218 },   // First y line
    { 680 - 240 - 46, 296, 46, 30 },    // First x line
    { 680 - 198 - 26, 462, 26, 112 },   // Second y line
    { 680 - 198 - 92, 620, 92, 22 },    // First x line
    { 680 - 74 - 88, 620, 88, 24 },     // First inversed L x axis
    { 680 - 140 - 22, 640, 22, 92 },    // First inversed L y axis
    { 680 - 198 - 26, 680, 26, 120 },   // Second y line
    { 680 - 74 - 212, 780, 212, 24 },    // Second y line
    // Middle of the map
    { 333, 30, 22, 130 }, // first middle line
    { 270, 202, 140, 62 }, // first rectangle line
    { 333, 214, 22, 112 }, // first middle line
    { 270, 678, 140, 62 }, // last rectangle line
    { 333, 690, 22, 112 }, // last middle line
    { 270, 522, 140, 62 }, // third rectangle line
    { 333, 530, 22, 112 }, // third middle line

    { 270, 368, 22, 112 }, // middle rectangle left
    { 388, 368, 22, 112 }, // middle rectangle right
    { 270, 458, 112, 22 }, // middle rectangle bottom
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

    bool allPelletsEaten = true;
    for (int i = 0; i < NUM_PELLETS; i++)
    {
        if (!pellets[i].eaten)
        {
            allPelletsEaten = false;
            break;
        }
    }

    // Set gameWon flag if all pellets are eaten
    if (allPelletsEaten)
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