#include "Player.h"

Player::Player() : entity(Loader::LoadModel("res/Models/hans.dae"), playerPos, playerRot, 0.3, "", false)
{
	
	//initialize physics
	btCollisionShape* playerCollider = new btBoxShape(btVector3(2, 2, 2));
	btDefaultMotionState* playerMotion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), Maths::glmToBullet(playerPos)));
	btScalar playerMass = 100;
	btVector3 playerFallInertia = btVector3(0, 0, 0);
	playerCollider->calculateLocalInertia(playerMass, playerFallInertia);

	btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(playerMass, playerMotion, playerCollider, playerFallInertia);
	playerRigidBody = new btRigidBody(playerRigidBodyCI);



}

void Player::Update(float dt) {

	CheckInput();
	btTransform trans;
	playerRigidBody->getMotionState()->getWorldTransform(trans);
	playerPos = vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
	entity.position = playerPos;
}


void Player::Render() {
	MasterRenderer::GetInstance().processEntity(&entity);
};

void Player::CheckInput()
{

	if (InputManager::GetInstance().IsKeyDown(SDLK_a)) {
		PlayerMove();
	};
	if (InputManager::GetInstance().IsKeyDown(SDLK_SPACE)) {
		PlayerJump();
	}
}

void Player::PlayerMove() {
	
	/*playerPos = vec3(playerPos.x, playerPos.y, playerPos.z - 0.001);
	playerRot = vec3(playerRot.x, playerRot.y + 5, playerRot.z);
	entity.position = playerPos;
	entity.rotation = playerRot;*/

	playerRigidBody->setLinearVelocity(btVector3(0, 0, 2));
	cout << "foi" << endl;
};

void Player::PlayerJump() {

	playerRigidBody->applyCentralImpulse(btVector3(0, 10, 0));
}