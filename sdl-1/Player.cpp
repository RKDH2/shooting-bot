#include "Player.h"
#include "stdio.h"

void InitPlayer(Player* player, int maxAmmo) {
	player->maxAmmo = maxAmmo;
	player->ammo = maxAmmo;
	player->isReloading = false;
	player->reloadStartTime = 0;
	player->lastFireTime = 0;
	printf("플레이어 초기화 완료! 장전된 총알: %d발\n", player->ammo);
}

// 총을 사용 가능한 상황인지
bool CanShoot(Player* player, Uint64 currentTime, int fireDelay) {
	if (player->isReloading) return false;
	if (player->ammo <= 0) return false;
	if (currentTime - player->reloadStartTime < fireDelay) return false; // 연사 딜레이

	return true;
}

// 총을 쐈을 경우
void ShootGun(Player* player, Uint64 currentTime) {
	player->ammo--;
	player->lastFireTime = currentTime;
	printf("빵! 남은 총알: %d / %d\n", player->ammo, player->maxAmmo);
}

// 재장전 시작
void StartReload(Player* player, Uint64 currentTime) {
	if (!player->isReloading && player->ammo < player->maxAmmo) {
		player->isReloading = true;
		player->reloadStartTime = currentTime;
		printf("철컥! 재장전 시작...\n");
	}
}

// 매 프레임마다 재장전이 끝났는지 체크
void UpdateReload(Player* player, Uint64 currentTime, int reloadDelay) {
	if (player->isReloading) {
		if (currentTime - player->reloadStartTime >= reloadDelay) {
			player->ammo = player->maxAmmo;
			player->isReloading = false;
			printf("재장전 완료!! 총알이 %d발 채워졌습니다.\n", player->ammo);
		}
	}
}