#pragma once
#include "includes.h"

typedef struct GameVar {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int lWidth; int lHeight;
    SDL_Rect buttonSave;
    SDL_Rect buttonUpgrade;
    SDL_Rect buttonReset;

    int running;
    int score;
    int scoreClick;
    int level;
    int upgradeCost;
} GameVar;
