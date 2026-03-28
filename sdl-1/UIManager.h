#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "Player.h"

struct UIAssets {
    TTF_Font* font;
    SDL_Texture* crosshairTex;
    SDL_Texture* pistolSelectionTex;
};

void RenderUI(SDL_Renderer* renderer, UIAssets* assets, Player* player, SDL_Rect crosshairRect);