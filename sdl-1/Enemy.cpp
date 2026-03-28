#include "Enemy.h"
#include "stdlib.h"

void InitEnemies(Enemy enemies[], int maxEnemies, SDL_Texture* texture) {
    for (int em = 0; em < maxEnemies; em++) {
        enemies[em].isActive = false;
        enemies[em].enemyTexture = texture;
    }
}

void SpawnEnemy(Enemy enemies[], int maxEnemies, int floorY[], int widths[], int heighrs[]) {
    for (int ct = 0; ct < maxEnemies; ct++) {
        if (!enemies[ct].isActive) {
            enemies[ct].isActive = true;
            enemies[ct].floor = rand() % 3;

            enemies[ct].hp = 3;

            // 적 사이즈
            int w = widths[enemies[ct].floor];
            int h = heighrs[enemies[ct].floor];

            // 적 생성 위치
            enemies[ct].size[0] = w;
            enemies[ct].size[1] = h;
            enemies[ct].x = rand() % (800 - w);
            enemies[ct].y = floorY[enemies[ct].floor] - h;

            break;
        }
    }
};

void RenderEnemies(SDL_Renderer* renderer, Enemy enemies[], int maxEnemies) {
    for (int em = 0; em < maxEnemies; em++) {
        if (enemies[em].isActive) {
            SDL_Rect enemyRect = { enemies[em].x, enemies[em].y, enemies[em].size[0], enemies[em].size[1] };
            if (enemies[em].enemyTexture != NULL) {
                SDL_RenderCopy(renderer, enemies[em].enemyTexture, NULL, &enemyRect);
            }
        }
    }
};

void ShootEnemy(Enemy enemies[], int maxEnemies, int mouseX, int mouseY) {
    SDL_Point clickPoint = { mouseX, mouseY };

    for (int em = 0; em < maxEnemies; em++) {
        if (enemies[em].isActive) {
            // 적 위치 범위
            SDL_Rect enemyRect = { enemies[em].x, enemies[em].y, enemies[em].size[0], enemies[em].size[1] };
            if (SDL_PointInRect(&clickPoint, &enemyRect)) {
                enemies[em].hp--;

                if (enemies[em].hp <= 0) {
                    enemies[em].isActive = false;
                }

                break;
            }
        }
    }
}