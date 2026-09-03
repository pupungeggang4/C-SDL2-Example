#pragma once
#include "includes.h"

typedef struct GameVar {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int lWidth; int lHeight;

    int running;
    int score;
    int scoreClick[10];
    int level;
    int upgradeCost[10];
} GameVar;

extern GameVar gameVar;
