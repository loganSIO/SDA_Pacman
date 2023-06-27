#include "game.h"
#include "graphics.h"
#include "utils.h"

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
