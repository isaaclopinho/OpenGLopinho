#pragma once

#include "../PhysicsObject.h"
#include "../MasterRenderer.h"
#include "../InputManager.h"
#include "../Game.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

class Player : public PhysicsObject {
	
public:
	Entity entity;
	Player();
	~Player();
	void Update(float dt);
	void Render();

	vec3 getPlayerPos() { return playerPos; };
	vec3 getPlayerRot() { return playerRot; };

	btRigidBody* getRB() override {return playerRigidBody;};

	static Player* getInstance();

private:

	static Player* instance;
	float turnAngle, maxSpeed, maxMoveForce;
	void CheckCoolDowns();
	void CheckInput();
	void PlayerJump();
	void PlayerMove(float horizontalInput, float verticalInput, int newRot);

	btRigidBody* playerRigidBody;

	bool canJump;
	float jumpCoolDown, jumpTimeStamp;
	vec3 playerPos = vec3(0, 10, 0);
	vec3 playerRot = vec3(-90, 0, 0);
	int walkSpeed;

	void ControlSpeed();
	void AnimationController();

	float dt;
};
