#include "GameCamera.h"

void GameCamera::CameraFollow() {

	vec3 desiredPosition = player->getPlayerPos() + cameraFollowOffset;
	btVector3 smoothedPosition = lerp(Maths::glmToBullet(position), Maths::glmToBullet(desiredPosition), smoothSpeed);
	if (distance(desiredPosition, Maths::bulletToGlm(smoothedPosition)) > 0.05f) {
		position = Maths::bulletToGlm(smoothedPosition);
	}


	//CheckOffset();
	
	
	
	cout << position.x << " " << position.y << " " << position.z << endl;
	//cout << position.x << " " << position.y << " " << position.z << endl;
};

void GameCamera::Update() {

	CameraFollow();
};

void GameCamera::CheckOffset(vec3 playerPos) {

	if (playerPos.z > position.z -	 cameraFollowOffset.z) {
		position.z += 1;
		cout << "passou" << endl;
	}
};