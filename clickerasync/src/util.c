#include "util.h"
#include "gamevar.h"
#include "game.h"

#ifdef __EMSCRIPTEN__
EM_ASYNC_JS(void, syncDB, (int to_memfs), {
    await new Promise((resolve) => {
        FS.syncfs(to_memfs, (err) => {
            if (err) Module.print("동기화 에러:", err);
			else Module.print("동기화 성공");
            resolve();
        });
    });
});
#endif
