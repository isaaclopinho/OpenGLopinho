#pragma once

#include "../PhysicsObject.h"
#include "../MasterRenderer.h"
#include "../InputManager.h"
#include "../Game.h"
#include "../Timer.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

class Player : public PhysicsObject {
	
public:
	Entity entity;
	Player();
	~Player();
	void Update(float dt);
    void land();
	void Render();
    vec3 getPlayerPos() { return playerPos; };
    vec3 getPlayerRot() { return playerRot; };
    void SetHP(int newHP);
    int GetHP();
    void SetMaxHP(int newMaxHP);
    int GetMaxHP();
    void LoseHP(int hpLoss, btVector3 origin);
	vec3 getForwardVector();
//    btRigidBody* getRB() override {return playerRigidBody;};
	static Player* getInstance();

private:

	static Player* instance;
	float turnAngle, maxSpeed, maxMoveForce;
	void CheckCoolDowns();
	void CheckInput();
	void PlayerJump();
	void PlayerMove(float horizontalInput, float verticalInput, int newRot);

	bool canJump;
//    float jumpCoolDown, jumpTimeStamp;
    Timer jump, invulneravel, ataque, knockback;
    vec3 playerPos = vec3(0, 10, 0); //starting position
    vec3 playerRot = vec3(-90, 0, 0); //starting rotation?
	int walkSpeed;
    int maxHP;
    int hp;

	float velocity, velocityStep, maxVelocity, minVelocity;

	void ControlSpeed();
	void AnimationController();

	float dt;
};
