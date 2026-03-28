#pragma once
#include "SDL.h"

struct Enemy {
    int floor;
    int x, y;
    int hp;
    int size[2];
    bool isActive;
    SDL_Texture* enemyTexture;
};

void InitEnemies(Enemy enemies[], int maxEnemies, SDL_Texture* texture);
void SpawnEnemy(Enemy enemies[], int maxEnemies, int floorY[], int widths[], int heighrs[]);
void RenderEnemies(SDL_Renderer* texture, Enemy enemies[], int maxEnemies);
void ShootEnemy(Enemy enemies[], int maxEnemies, int mouseX, int mouseY);