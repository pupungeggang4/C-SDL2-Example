#include "util.h"
#include "gamevar.h"
#include "game.h"

#ifdef __EMSCRIPTEN__
void init() {
    EM_ASM({
        try { FS.mkdir('/save'); } catch(e) {}
        FS.mount(FS.filesystems.IDBFS, {}, '/save');
        console.log("IDBFS 마운트 완료");
        FS.syncfs(true, function(err) {
            if (err) console.log(err);
            else console.log("Load complete!");
            _startGame();
        });
    });
}

void syncDB() {
    EM_ASM({
        if (window.isSaving) return;
        window.isSaving = true;
        console.log("Saving... do not close the tab.");

        FS.syncfs(false, function(err) {
            window.isSaving = false;
            if (err) console.log(err);
            else console.log("Save complete!");
        });
    });
}

EMSCRIPTEN_KEEPALIVE
void startGame() {
    initGame(&gameVar);
    emscripten_set_main_loop(loopGame, 0, 1);
}

void loopGame() {
    loop(&gameVar);
}
#endif
