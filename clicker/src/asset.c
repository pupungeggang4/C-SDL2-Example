#include "asset.h"

TTF_Font* font = NULL;
SDL_Texture *labelScore = NULL, *labelClick = NULL, *labelReset = NULL, *labelSave = NULL, *labelUpgrade = NULL, *labelScoreNum = NULL, *labelClickNum = NULL, *labelUpgradeNum = NULL;
SDL_Texture* tnum[10] = {NULL}; 
SDL_Rect buttonUpgrade = {620, 500, 160, 80}, buttonReset = {20, 500, 160, 80}, buttonSave = {620, 20, 160, 80};
SDL_Rect rLabelScore = {20, 30, 0, 0}, rLabelClick = {20, 70, 0, 0}, rLabelReset = {40, 510, 0, 0}, rLabelSave = {640, 30, 0, 0}, rLabelUpgrade = {640, 510, 0, 0};
SDL_Point textScoreNum = {140, 30}, textClickNum = {200, 70}, textUpgradeNum = {640, 550};
