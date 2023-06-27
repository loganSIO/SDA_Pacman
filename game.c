#include "game.h"
#include "graphics.h"
#include "utils.h"

#include <libgen.h>
#include <unistd.h>


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