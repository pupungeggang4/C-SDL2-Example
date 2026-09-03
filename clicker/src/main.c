#include "includes.h"
#include "gamevar.h"
#include "game.h"
#include "util.h"

void loopGame() {
    loop(&gameVar);
}

int main(int argc, char** argv) {
    #ifdef __EMSCRIPTEN__
    initDB();
    syncDB(1);
    initGame(&gameVar);
    emscripten_set_main_loop(loopGame, 0, 1);
    #else
    initGame(&gameVar);
    while(gameVar.running) {
        loop(&gameVar);
    }
    disposeAsset(&gameVar);
    #endif
    return 0;
}
