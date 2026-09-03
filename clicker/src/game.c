#include "game.h"
#include "asset.h"

void initGame(GameVar* gameVar) {
    // Init SDL2.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize SDL. %s\n", SDL_GetError());
        return;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        printf("Failed to initialize Image. %s\n", IMG_GetError());
        return;
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    int width, height;
    #ifdef __EMSCRIPTEN__
    width = 800; height = 600;
    #else
    SDL_DisplayMode dm;
    int mWidth, mHeight;
    if (SDL_GetCurrentDisplayMode(0, &dm) == 0) {
        mWidth = dm.w;
        mHeight = dm.h;
    } else {
        SDL_Log("Failed to get display mode: %s", SDL_GetError());
    }
    if (mWidth * 3 / 4 > mHeight) {
        height = (int)(mHeight * 0.8f);
        width = height * 4 / 3;
    } else {
        width = (int)(mWidth * 0.8f);
        height = width * 3 / 4;
    }
    #endif
    gameVar->window = SDL_CreateWindow("Clicker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    gameVar->renderer = SDL_CreateRenderer(gameVar->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(gameVar->renderer, gameVar->lWidth, gameVar->lHeight);
    gameVar->running = 1;

    gameVar->buttonUpgrade = (SDL_Rect){20, 500, 160, 80};
    gameVar->buttonReset = (SDL_Rect){620, 500, 160, 80};
    gameVar->buttonSave = (SDL_Rect){620, 20, 160, 80};

    FILE *f = fopen("save/save.txt", "r");
    if (!f) {
        FILE *temp = fopen("save/save.txt", "w");
        fprintf(temp, "0 1 1 5\n");
        fclose(temp);
        FILE *temp2 = fopen("save/save.txt", "r");
        fscanf(temp2, "%d %d %d %d", &gameVar->score, &gameVar->scoreClick, &gameVar->level, &gameVar->upgradeCost);
        fclose(temp);
    } else {
        fscanf(f, "%d %d %d %d", &gameVar->score, &gameVar->scoreClick, &gameVar->level, &gameVar->upgradeCost);
    }
    fclose(f);
}

void loop(GameVar* gameVar) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            gameVar->running = 0;
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            SDL_Point pos = {e.button.x, e.button.y};
            //printf("%d, %d\n", pos.x, pos.y);
            printf("%d\n", gameVar->scoreClick);
        }
    }

    SDL_SetRenderDrawColor(gameVar->renderer, 255, 255, 127, 255);
    SDL_RenderClear(gameVar->renderer);
    render(gameVar);
    SDL_RenderPresent(gameVar->renderer);

    #ifdef __EMSCRIPTEN__
    if (!gameVar->running) {
        emscripten_cancel_main_loop();
    }
    #endif
}

void render(GameVar* gameVar) {
    SDL_SetRenderDrawColor(gameVar->renderer, 0, 255, 127, 255);
    SDL_RenderFillRect(gameVar->renderer, &gameVar->buttonUpgrade);
    SDL_SetRenderDrawColor(gameVar->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(gameVar->renderer, &gameVar->buttonReset);
    SDL_SetRenderDrawColor(gameVar->renderer, 0, 255, 255, 255);
    SDL_RenderFillRect(gameVar->renderer, &gameVar->buttonSave);
}

void loadAsset(GameVar* gameVar) {
    
}

void disposeAsset(GameVar* gameVar) {
}
