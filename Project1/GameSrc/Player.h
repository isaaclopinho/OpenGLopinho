#pragma once

#include "..\GameObject.h"
#include "..\MasterRenderer.h"
#include "..\InputManager.h"


class Player : public GameObject {
public:
	Entity entity;
	Player();
	~Player();
	void Update(float dt);
	void Render();

	vec3 playerPos = vec3(0, 0, 0);
	vec3 playerRot = vec3(0, 180, 0);

private:
	
	void CheckInput();
	void PlayerMove();
};