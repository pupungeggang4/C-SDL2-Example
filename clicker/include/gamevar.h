#pragma once
#include "includes.h"

typedef struct GameVar {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int lWidth; int lHeight;

    int running;
    int score;
    int scoreClick;
    int level;
    int upgradeCost;
} GameVar;

extern GameVar gameVar;
