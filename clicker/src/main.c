#include "includes.h"
#include "gamevar.h"
#include "game.h"
#include "util.h"

int main(int argc, char** argv) {
    #ifdef __EMSCRIPTEN__
    init();
    #else
    initGame(&gameVar);
    while(gameVar.running) {
        loop(&gameVar);
    }
    disposeAsset(&gameVar);
    #endif
    return 0;
}
