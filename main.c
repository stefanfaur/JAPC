#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 32;
const int GRID_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int GRID_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;

// SDL variables
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

// Pacman variables
SDL_Rect pacman;
int pacmanSpeed = 1;

// Maze variables
int maze[GRID_HEIGHT][GRID_WIDTH];

// Function prototypes
bool init();
void close();
void movePacman();
void drawPacman();
void generateMaze();
bool canMove(int x, int y);
void drawMaze();

int main(int argc, char *args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        srand((unsigned int)time(NULL));

        pacman.x = (SCREEN_WIDTH - TILE_SIZE) / 2;
        pacman.y = (SCREEN_HEIGHT - TILE_SIZE) / 2;
        pacman.w = TILE_SIZE;
        pacman.h = TILE_SIZE;
        generateMaze();

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

            drawMaze();
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

void drawPacman() {
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
    SDL_RenderFillRect(gRenderer, &pacman);
}

void movePacman() {
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    int newX = pacman.x, newY = pacman.y;

    if (keystate[SDL_SCANCODE_UP]) {
        newY -= pacmanSpeed;
    }
    if (keystate[SDL_SCANCODE_DOWN]) {
        newY += pacmanSpeed;
    }
    if (keystate[SDL_SCANCODE_LEFT]) {
        newX -= pacmanSpeed;
    }
    if (keystate[SDL_SCANCODE_RIGHT]) {
        newX += pacmanSpeed;
    }

    if (canMove(newX, pacman.y)) {
        pacman.x = newX;
    }

    if (canMove(pacman.x, newY)) {
        pacman.y = newY;
    }
}

void generateMaze() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (y == 0 || y == GRID_HEIGHT - 1 || x == 0 || x == GRID_WIDTH - 1) {
                maze[y][x] = 1;
            } else {
                maze[y][x] = (rand() % 100 < 20) ? 1 : 0;
            }
        }
    }
}

bool canMove(int x, int y) {
    int leftTileX = (x) / TILE_SIZE;
    int rightTileX = (x + pacman.w - 1) / TILE_SIZE;
    int topTileY = (y) / TILE_SIZE;
    int bottomTileY = (y + pacman.h - 1) / TILE_SIZE;

    return maze[topTileY][leftTileX] == 0 &&
           maze[topTileY][rightTileX] == 0 &&
           maze[bottomTileY][leftTileX] == 0 &&
           maze[bottomTileY][rightTileX] == 0;
}

void drawMaze() {
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (maze[y][x] == 1) {
                SDL_Rect wall = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_RenderFillRect(gRenderer, &wall);
            }
        }
    }
}

