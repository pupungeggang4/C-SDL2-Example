#include "util.h"
#include "gamevar.h"

#ifdef __EMSCRIPTEN__
void initDB() {
    EM_ASM({
        try { FS.mkdir('/save'); } catch(e) {}
        FS.mount(FS.filesystems.IDBFS, {}, '/save');
        console.log("IDBFS 마운트 완료");
    });
}

EM_ASYNC_JS(void, syncDB, (int to_memfs), {
    await new Promise((resolve) => {
        FS.syncfs(to_memfs, (err) => {
            if (err) console.error("동기화 에러:", err);
            resolve();
        });
    });
});
#endif
