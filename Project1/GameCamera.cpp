#include "GameCamera.h"

void GameCamera::CameraFollow() {

	vec3 newCameraPos = player->getPlayerPos();
	position = vec3(newCameraPos.x, newCameraPos.y, newCameraPos.z + 5);
	cout << newCameraPos.x << " " << newCameraPos.y << " " << newCameraPos.z << endl;
};

void GameCamera::Update() {

	CameraFollow();
};