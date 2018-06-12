#include "GameCamera.h"

void GameCamera::CameraFollow() {

	vec3 desiredPosition = player->getPlayerPos() + cameraFollowOffset;
	btVector3 smoothedPosition = lerp(Maths::glmToBullet(position), Maths::glmToBullet(desiredPosition), smoothSpeed);
	if (distance(desiredPosition, Maths::bulletToGlm(smoothedPosition)) > 0.05f) {
		position = Maths::bulletToGlm(smoothedPosition);
	}
};

void GameCamera::Update() {

	CheckOffset();
};

void GameCamera::CheckOffset() {

	if (player->getPlayerPos().z > position.z -	 cameraFollowOffset.z) {
		CameraFollow();
	}
};