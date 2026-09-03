#include "game.h"
#include "asset.h"
#include "util.h"

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
    loadAsset(gameVar);

    gameVar->running = 1;

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
        } else if (e.type == SDL_MOUSEBUTTONUP) {
            SDL_Point pos = {e.button.x, e.button.y};
            if (SDL_PointInRect(&pos, &buttonReset)) {
                FILE *temp = fopen("save/save.txt", "w");
                fprintf(temp, "0 1 1 5\n");
                fclose(temp);
                FILE *temp2 = fopen("save/save.txt", "r");
                fscanf(temp2, "%d %d %d %d", &gameVar->score, &gameVar->scoreClick, &gameVar->level, &gameVar->upgradeCost);
                fclose(temp);
                #ifdef __EMSCRIPTEN__
                syncDB(0);
                #endif
            } else if (SDL_PointInRect(&pos, &buttonSave)) {
                FILE *temp = fopen("save/save.txt", "w");
                fprintf(temp, "%d %d %d %d\n", gameVar->score, gameVar->scoreClick, gameVar->level, gameVar->upgradeCost);
                fclose(temp);
                #ifdef __EMSCRIPTEN__
                syncDB(0);
                #endif
            } else {
                gameVar->score += gameVar->scoreClick;
            }
            printf("%d %d %d %d\n", gameVar->score, gameVar->scoreClick, gameVar->level, gameVar->upgradeCost);
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
    SDL_SetRenderDrawColor(gameVar->renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(gameVar->renderer, &buttonUpgrade);
    SDL_SetRenderDrawColor(gameVar->renderer, 255, 127, 127, 255);
    SDL_RenderFillRect(gameVar->renderer, &buttonReset);
    SDL_RenderCopy(gameVar->renderer, labelReset, NULL, &rLabelReset);
    SDL_SetRenderDrawColor(gameVar->renderer, 255, 0, 255, 255);
    SDL_RenderFillRect(gameVar->renderer, &buttonSave);
}

void loadAsset(GameVar* gameVar) {
    font = TTF_OpenFont("asset/neodgm.ttf", 32);
    SDL_Color textColor = {0, 0, 0, 255};

    SDL_Surface* sLabelReset = TTF_RenderText_Blended(font, "Reset", textColor);
    labelReset = SDL_CreateTextureFromSurface(gameVar->renderer, sLabelReset);
    rLabelReset.w = sLabelReset->w;
    rLabelReset.h = sLabelReset->h;
    SDL_FreeSurface(sLabelReset);

    for (int i = 0; i < 10; i++) {
        char text[2];
        snprintf(text, sizeof(text), "%d", i);
        SDL_Surface* temp = TTF_RenderText_Blended(font, text, textColor);
        tnum[i] = SDL_CreateTextureFromSurface(gameVar->renderer, temp);
        SDL_FreeSurface(temp);
    }
}

void disposeAsset(GameVar* gameVar) {
    SDL_DestroyRenderer(gameVar->renderer);
    SDL_DestroyWindow(gameVar->window);
}
