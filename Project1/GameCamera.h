#pragma once
#include <glm/glm.hpp>
#include "Maths.h"
#include "Camera.h"
#include "GameSrc\Player.h"

class GameCamera : public Camera {

public:

	GameCamera(vec3 pos) { position = pos; yaw = 0; pitch = 0; roll = 0; cameraFollowOffset = vec3(0, 1, 3); smoothSpeed = 0.125f; };
	void Update();
	
private:
	void CameraFollow();
	void CheckOffset();

	Player* player = Player::getInstance();

	vec3 cameraFollowOffset;
	btScalar smoothSpeed;
};