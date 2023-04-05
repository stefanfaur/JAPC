#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 32;

// SDL variables
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

// Pacman variables
SDL_Rect pacman;
int pacmanSpeed = 1;

// Function prototypes
bool init();
void close();
void movePacman();
void drawPacman();

int main(int argc, char *args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        srand((unsigned int)time(NULL));

        pacman.x = (SCREEN_WIDTH - TILE_SIZE) / 2;
        pacman.y = (SCREEN_HEIGHT - TILE_SIZE) / 2;
        pacman.w = TILE_SIZE;
        pacman.h = TILE_SIZE;

        bool quit = false;
        SDL_Event e;

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            movePacman();

            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
            SDL_RenderClear(gRenderer);

            drawPacman();

            SDL_RenderPresent(gRenderer);
        }
    }

    close();
    return 0;
}

// Function implementations
bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    SDL_Quit();
}

void movePacman() {
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_UP]) {
        pacman.y -= pacmanSpeed;
    }
    if (keystate[SDL_SCANCODE_DOWN]) {
        pacman.y += pacmanSpeed;
    }
    if (keystate[SDL_SCANCODE_LEFT]) {
        pacman.x -= pacmanSpeed;
    }
    if (keystate[SDL_SCANCODE_RIGHT]) {
        pacman.x += pacmanSpeed;
    }
}

void drawPacman() {
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
    SDL_RenderFillRect(gRenderer, &pacman);
}
