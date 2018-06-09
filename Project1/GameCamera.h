#pragma once
#include <glm/glm.hpp>
#include "Camera.h"
#include "GameSrc\Player.h"

class GameCamera : public Camera {

	void CameraMove();
	void Update();
};