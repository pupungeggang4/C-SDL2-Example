#include "includes.h"
#include "gamevar.h"

GameVar gameVar = {0};
#ifdef __EMSCRIPTEN__
void initDB() {
    EM_ASM({
        try { FS.mkdir('/save'); } catch(e) {}
        FS.mount(FS.filesystems.IDBFS, {}, '/save');
        console.log("IDBFS 마운트 완료");
    });
}

EM_ASYNC_JS(void, syncDB, (bool to_memfs), {
    await new Promise((resolve) => {
        FS.syncfs(to_memfs, (err) => {
            if (err) console.error("동기화 에러:", err);
            resolve();
        });
    });
});

void loopGame() {
    //loop(gameVar);
}
#endif

int main(int argc, char** argv) {
    #ifdef __EMSCRIPTEN__
    initDB();
    syncDB(true);
    emscripten_set_main_loop(loopGame, 0, 1);
    #else

    #endif
    return 0;
}
