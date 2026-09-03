#include "asset.h"

TTF_Font* font = NULL;
SDL_Texture *labelScore = NULL, *labelReset = NULL, *labelSave = NULL, *labelUpgrade = NULL;
SDL_Texture* tnum[10] = {NULL}; 
SDL_Rect buttonUpgrade = {620, 500, 160, 80}, buttonReset = {20, 500, 160, 80}, buttonSave = {620, 20, 160, 80};
SDL_Rect rLabelScore = {20, 20, 0, 0}, rLabelReset = {40, 520, 0, 0}, rLabelSave = {640, 20, 0, 0}, rLabelUpgrade = {640, 20, 0, 0};
