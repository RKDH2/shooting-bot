#include "SDL.h"
#include "SDL_image.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "Enemy.h"
#include "Player.h"
#include "SDL_ttf.h"
#include "UIManager.h"

enum GameStage {
    STAGE_1,
    STAGE_2,
    STAGE_3,
    BOSS_STAGE,
    GAME_CLEAR
};

struct StageInfo {
    GameStage currentState;
    int floorY[3];
    int targetKills;
    int currentKills;
    int enemySpawnDelay;
    SDL_Texture* backgroundTexture;
};

int main(int argc, char* argv[]) {
    srand((unsigned int)time(NULL));

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("shooting_bot", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_ShowCursor(SDL_DISABLE);

    StageInfo stage_1;
    stage_1.currentState = STAGE_1;
    stage_1.floorY[0] = 68; // 3층
    stage_1.floorY[1] = 240; // 2층
    stage_1.floorY[2] = 500; // 1층
    stage_1.backgroundTexture = IMG_LoadTexture(renderer, "recources/images/background-stage-1.png");

    UIAssets uiAssets;
    uiAssets.font = TTF_OpenFont("recources/fonts/consola.ttf", 14);
    uiAssets.crosshairTex = IMG_LoadTexture(renderer, "recources/images/crosshair.png");
    uiAssets.pistolSelectionTex = IMG_LoadTexture(renderer, "recources/images/pistol-selection.png");


    SDL_Texture* commonEnemyTex = IMG_LoadTexture(renderer, "recources/images/enemy-common.png");

    // 게임 오브젝트 초기화
    const int MAX_ENEMIES = 5;
    Enemy enemies[MAX_ENEMIES];
    InitEnemies(enemies, MAX_ENEMIES, commonEnemyTex);

    int enemieWidths[3] = { 15, 25, 35 };
    int enemieHeights[3] = { 15, 25, 35 };

    Player player;
    InitPlayer(&player, 10);

    Uint64 lastSpawnTime = 0;
    int spawnDelay = 1500;
    bool quit = false;
    SDL_Event e;
    SDL_Rect crosshairRect = { 0, 0, 110, 110 };

    while (!quit) {
        Uint64 currentTime = SDL_GetTicks64();

        // 입력 처리
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r) {
                 StartReload(&player, currentTime);
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                if (CanShoot(&player, currentTime, 100)) {
                    ShootGun(&player, currentTime);
                    ShootEnemy(enemies, MAX_ENEMIES, e.button.x, e.button.y);
                }
            }
        }

        // 상태 업데이트
        if (player.ammo <= 0 && !player.isReloading) StartReload(&player, currentTime);
        UpdateReload(&player, currentTime, 2000);

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        crosshairRect.x = mouseX - (crosshairRect.w / 2);
        crosshairRect.y = mouseY - (crosshairRect.h / 2);

        if (currentTime - lastSpawnTime > spawnDelay) {
            SpawnEnemy(enemies, MAX_ENEMIES, stage_1.floorY, enemieWidths, enemieHeights);
            lastSpawnTime = currentTime;
        }

        // 화면 그리기
        SDL_RenderClear(renderer);

        if (stage_1.backgroundTexture) SDL_RenderCopy(renderer, stage_1.backgroundTexture, NULL, NULL);

        // 가이드라인 그리기
        //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        //SDL_RenderDrawLine(renderer, 0, stage_1.floorY[0], 800, stage_1.floorY[0]);
        //SDL_RenderDrawLine(renderer, 0, stage_1.floorY[1], 800, stage_1.floorY[1]);
        //SDL_RenderDrawLine(renderer, 0, stage_1.floorY[2], 800, stage_1.floorY[2]);

        RenderEnemies(renderer, enemies, MAX_ENEMIES); // 적 그리기

        RenderUI(renderer, &uiAssets, &player, crosshairRect); // 조준점 이미지 그리기

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(commonEnemyTex);
    SDL_DestroyTexture(uiAssets.crosshairTex);
    SDL_DestroyTexture(uiAssets.pistolSelectionTex);
    SDL_DestroyTexture(stage_1.backgroundTexture);
    TTF_CloseFont(uiAssets.font);

    IMG_Quit();
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}