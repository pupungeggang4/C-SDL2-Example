#pragma once
#include "includes.h"

#ifdef __EMSCRIPTEN__
void initDB();
void syncDB();
void startGame();
void loopGame();
#endif
