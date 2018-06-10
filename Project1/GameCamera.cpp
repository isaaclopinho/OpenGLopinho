#include "GameCamera.h"

void GameCamera::CameraMove() {

	player->getPlayerPos();
};

void GameCamera::Update() {

	CameraMove();
};