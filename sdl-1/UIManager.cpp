#include "UIManager.h"
#include <stdio.h>

void RenderUI(SDL_Renderer* renderer, UIAssets* assets, Player* player, SDL_Rect crosshairRect) {
    // 조준점 그리기
    if (assets->crosshairTex != NULL && !player->isReloading) {
        SDL_RenderCopy(renderer, assets->crosshairTex, NULL, &crosshairRect);
    }

    // 총 선택창 그리기
    SDL_Rect gunSelectionRect = { 10, 510, 70, 80 };
    if (assets->pistolSelectionTex != NULL) {
        SDL_RenderCopy(renderer, assets->pistolSelectionTex, NULL, &gunSelectionRect);
    }

    // 총알 텍스트 만들기 및 그리기
    char ammoText[64];
    if (player->isReloading) {
        sprintf_s(ammoText, "-- / --");
    }
    else {
        sprintf_s(ammoText, "%d / %d", player->ammo, player->maxAmmo);
    }

    if (assets->font != NULL) {
        SDL_Color textColor = { 0, 0, 0, 255 };
        SDL_Surface* textSurface = TTF_RenderText_Blended(assets->font, ammoText, textColor);

        if (textSurface != NULL) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture != NULL) {
                int textX = gunSelectionRect.x + (gunSelectionRect.w / 2) - (textSurface->w / 2); // 가로 중앙 정렬
                SDL_Rect textRect = { textX, 570, textSurface->w, textSurface->h };
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture); // 메모리 누수 방지
            }
            SDL_FreeSurface(textSurface); // 메모리 누수 방지
        }
    }
}