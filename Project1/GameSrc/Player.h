#pragma once

#include "..\GameObject.h"
#include "..\MasterRenderer.h"
#include "..\InputManager.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

class Player : public GameObject {
public:
	Entity entity;
	Player();
	~Player();
	void Update(float dt);
	void Render();

	vec3 playerPos = vec3(0, 90, -5);
	vec3 playerRot = vec3(0, 90, 0);

	btRigidBody* getRB() {
		return playerRigidBody;
	};

private:
	
	void CheckInput();
	void PlayerMove();

	btRigidBody* playerRigidBody;
};