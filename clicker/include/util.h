#pragma once
#include "includes.h"

#ifdef __EMSCRIPTEN__
void init();
void syncDB();
void startGame();
void loopGame();
#endif
