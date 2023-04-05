#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 32;
const int GRID_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int GRID_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;
const int DOT_COUNT = 100;

// SDL variables
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

// Dot variables
SDL_Rect dots[DOT_COUNT];

// Pacman variables
SDL_Rect pacman;
int pacmanSpeed = 1;
int score = 0;

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
void findSpawnPoint(int *x, int *y);
void generateDots();
void checkDotCollision();
void drawDots();
void renderText(const char *text, int x, int y, SDL_Color color);

int main(int argc, char *args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        srand((unsigned int)time(NULL));

        generateMaze();
        int spawnX, spawnY;
        findSpawnPoint(&spawnX, &spawnY);
        pacman.x = spawnX;
        pacman.y = spawnY;
        pacman.w = TILE_SIZE;
        pacman.h = TILE_SIZE;
        generateDots();

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
            checkDotCollision();
            drawDots();
            drawPacman();
            char scoreText[20];
            sprintf(scoreText, "Score %d", score);
            SDL_Color whiteColor = {255, 255, 255, 255};
            renderText(scoreText, 10, 10, whiteColor);


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
    //Call TTF_Init() before using the functions
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s \n", TTF_GetError());
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

void findSpawnPoint(int *x, int *y) {
    bool found = false;
    while (!found) {
        int spawnX = rand() % (GRID_WIDTH - 2) + 1;
        int spawnY = rand() % (GRID_HEIGHT - 2) + 1;
        if (maze[spawnY][spawnX] == 0) {
            *x = spawnX * TILE_SIZE;
            *y = spawnY * TILE_SIZE;
            found = true;
        }
    }
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

void generateDots() {
    for (int i = 0; i < DOT_COUNT; i++) {
        int dotX, dotY;
        findSpawnPoint(&dotX, &dotY);
        dots[i].x = dotX + TILE_SIZE / 2 - (TILE_SIZE / 4) / 2;
        dots[i].y = dotY + TILE_SIZE / 2 - (TILE_SIZE / 4) / 2;
        dots[i].w = TILE_SIZE / 4;
        dots[i].h = TILE_SIZE / 4;
    }
}


void checkDotCollision() {
    for (int i = 0; i < DOT_COUNT; i++) {
        if (dots[i].w != 0 && SDL_HasIntersection(&pacman, &dots[i])) {
            dots[i].w = 0;
            dots[i].h = 0;
            score += 10;
        }
    }
}


void drawDots() {
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    for (int i = 0; i < DOT_COUNT; i++) {
        if (dots[i].w != 0) {
            SDL_RenderFillRect(gRenderer, &dots[i]);
        }
    }
}

void renderText(const char *text, int x, int y, SDL_Color color) {
    char *basePath = SDL_GetBasePath();
    if (!basePath) {
        printf("SDL_GetBasePath: %s\n", SDL_GetError());
        return;
    }

    char fontPath[256];
    strcpy(fontPath, basePath);
    strcat(fontPath, "assets/ARCADECLASSIC.TTF");

    TTF_Font *font = TTF_OpenFont(fontPath, 24);
    if (!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};

    SDL_RenderCopy(gRenderer, textTexture, NULL, &textRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
    SDL_free(basePath);
}



