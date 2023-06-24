#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

SDL_Window* pWindow = NULL;
SDL_Surface* win_surf = NULL;
SDL_Surface* plancheSprites = NULL;

SDL_Rect src_bg = { 200, 3, 168, 216 };
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

int count;

SDL_Rect pacman = { 340 - (16 / 2), 650 - (16 / 2), 32, 32 };
int lastDirection = -1; // Store the last key pressed direction

#define NUM_WALLS 32

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
    { 198, 462, 26, 112 }, // first y line
    { 198, 620, 92, 22 }, // first x line
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
    { 680 - 74 - 212, 780, 212, 24 }    // Second y line
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

    // No collision detected
    return false;
}

void init()
{
    pWindow = SDL_CreateWindow("PacMan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 700, 900, SDL_WINDOW_SHOWN);
    win_surf = SDL_GetWindowSurface(pWindow);

    plancheSprites = SDL_LoadBMP("./pacman_sprites.bmp");
    count = 0;
}

void draw()
{
    SDL_SetColorKey(plancheSprites, false, 0);
    SDL_BlitScaled(plancheSprites, &src_bg, win_surf, &bg);

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

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    int direction = -1; // -1 indicates no direction

    if (keys[SDL_SCANCODE_LEFT])
    {
        direction = SDL_SCANCODE_LEFT;
    }
    else if (keys[SDL_SCANCODE_RIGHT])
    {
        direction = SDL_SCANCODE_RIGHT;
    }
    else if (keys[SDL_SCANCODE_UP])
    {
        direction = SDL_SCANCODE_UP;
    }
    else if (keys[SDL_SCANCODE_DOWN])
    {
        direction = SDL_SCANCODE_DOWN;
    }

    SDL_Rect pacman_direction;
    if (direction == SDL_SCANCODE_LEFT)
    {
        pacman_direction = pacman_left;
    }
    else if (direction == SDL_SCANCODE_RIGHT)
    {
        pacman_direction = pacman_right;
    }
    else if (direction == SDL_SCANCODE_UP)
    {
        pacman_direction = pacman_up;
    }
    else if (direction == SDL_SCANCODE_DOWN)
    {
        pacman_direction = pacman_down;
    }
    else
    {
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
    }

    if (direction != -1)
    {
        SDL_Rect nextPosition = pacman;
        switch (direction)
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
            lastDirection = direction;
        }
    }

    SDL_BlitScaled(plancheSprites, &pacman_direction, win_surf, &pacman);
}

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Echec de l'initialisation de la SDL %s", SDL_GetError());
        return 1;
    }

    init();

    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        while (!quit && SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            default:
                break;
            }
        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_ESCAPE])
            quit = true;

        draw();

        SDL_Delay(1);
        SDL_UpdateWindowSurface(pWindow);
    }

    SDL_Quit();
    return 0;
}
