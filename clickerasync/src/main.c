#include "includes.h"
#include "gamevar.h"
#include "game.h"
#include "util.h"

#ifdef __EMSCRIPTEN__
void loopGame() {
    loop(&gameVar);
}
#endif

int main(int argc, char** argv) {
    #ifdef __EMSCRIPTEN__
    EM_ASM({
        try { FS.mkdir('/save'); } catch(e) {}
        FS.mount(FS.filesystems.IDBFS, {}, '/save');
        Module.print("IDBFS 마운트 완료");
    });
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
