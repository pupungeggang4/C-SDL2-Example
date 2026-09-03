#include "util.h"
#include "gamevar.h"
#include "game.h"

#ifdef __EMSCRIPTEN__
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
#endif
