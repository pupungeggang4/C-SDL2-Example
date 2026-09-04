#include "includes.h"
#include "gamevar.h"
#include "game.h"
#include "util.h"

#ifdef __EMSCRIPTEN__
void loopGame() {
    loop(&gameVar);
}

EMSCRIPTEN_KEEPALIVE
void startGame() {
    initGame(&gameVar);
    emscripten_set_main_loop(loopGame, 0, 1);
}
#endif

int main(int argc, char** argv) {
    #ifdef __EMSCRIPTEN__
    EM_ASM({
        try { FS.mkdir('/save'); } catch(e) {}
        FS.mount(FS.filesystems.IDBFS, {}, '/save');
        Module.print("IDBFS 마운트 완료");
        FS.syncfs(true, function(err) {
            if (err) Module.print(err);
            else Module.print("Load complete!");
            _startGame();
        });
    });
    #else
    initGame(&gameVar);
    while(gameVar.running) {
        loop(&gameVar);
    }
    disposeAsset(&gameVar);
    #endif
    return 0;
}
