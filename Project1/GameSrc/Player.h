#pragma once

#include "..\PhysicsObject.h"
#include "..\MasterRenderer.h"
#include "..\InputManager.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

class Player : public PhysicsObject {
public:
	Entity entity;
	Player();
	~Player();
	void Update(float dt);
	void Render();

	vec3 playerPos = vec3(0, 20, -5);
	vec3 playerRot = vec3(0, 90, 0);

	btRigidBody* getRB() override {return playerRigidBody;};

private:
	
	void CheckInput();
	void PlayerJump();
	void PlayerMove();

	btRigidBody* playerRigidBody;
};