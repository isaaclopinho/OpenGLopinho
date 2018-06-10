#pragma once
#include <glm/glm.hpp>
#include "Camera.h"
#include "GameSrc\Player.h"

class GameCamera : public Camera {

public:

	GameCamera(vec3 pos) { position = pos; yaw = 0; pitch = 0; roll = 0; };
	void Update();

private:
	void CameraFollow();
	Player* player = Player::getInstance();
};