#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emscripten.h>
#include <stdio.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Global pointers for the browser main loop context
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
SDL_Rect destRect;

// Separate cleanup function to free VRAM and RAM resources properly
void cleanup() {
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

// Frame execution loop (called by browser requestAnimationFrame)
void main_loop() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            emscripten_cancel_main_loop();
            cleanup(); // Clean up right before exiting
            return;
        }
    }

    // Clear screen to black background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw texture to full target destination rectangle
    SDL_RenderCopy(renderer, texture, NULL, &destRect);

    // Present render buffer to screen
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    // Initialize SDL subsystems
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        SDL_Quit();
        return 1;
    }

    window = SDL_CreateWindow("SDL2 Emscripten Example", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load asset from Emscripten's virtual file system
    SDL_Surface* loadedSurface = IMG_Load("image.png");
    if (!loadedSurface) {
        cleanup();
        return 1;
    }

    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface); // Surface can be safely deleted

    if (!texture) {
        cleanup();
        return 1;
    }

    // Center the 400x300 image inside the 800x600 viewport
    destRect.w = 400;
    destRect.h = 300;
    destRect.x = (SCREEN_WIDTH - destRect.w) / 2;
    destRect.y = (SCREEN_HEIGHT - destRect.h) / 2;

    // 0 FPS matches the monitor refresh rate via requestAnimationFrame.
    // The '1' argument simulates an infinite loop blocking further execution in main.
    emscripten_set_main_loop(main_loop, 0, 1);

    return 0;
}

