#pragma once
#include "SDL.h"

struct Player {
    int ammo;
    int maxAmmo;
    bool isReloading;
    Uint64 reloadStartTime;
    Uint64 lastFireTime;
};

void InitPlayer(Player* player, int maxAmmo);
bool CanShoot(Player* player, Uint64 currentTime, int fireDelay);
void ShootGun(Player* player, Uint64 currentTime);
void StartReload(Player* player, Uint64 currentTime);
void UpdateReload(Player* player, Uint64 currentTime, int reloadDelay);