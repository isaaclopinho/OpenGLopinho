#include "Player.h"
#include <glm/gtx/rotate_vector.hpp>
Player* Player::instance = 0;

Player::Player() : entity(Loader::LoadModel("res/Models/hans_mesh2.dae"), playerPos, playerRot, 0.3, "Walk", true)
{
	btTransform initTransform;
	initTransform.setIdentity();
	initTransform.setOrigin(btVector3(0, 10, 0));
	//initialize physics
	btConvexShape* playerCollider = new btCapsuleShape(0.05f, 0.03f);
	btDefaultMotionState* playerMotion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), Maths::glmToBullet(playerPos)));
	btScalar playerMass = 1;
	btVector3 playerFallInertia = btVector3(0, 0, 0);
	playerCollider->calculateLocalInertia(playerMass, playerFallInertia);
	btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(playerMass, playerMotion, playerCollider, playerFallInertia);
	playerRigidBody = new btRigidBody(playerRigidBodyCI);
	
	//Initialize Player Variables
	canJump = true;
	jumpTimeStamp = 0;
	jumpCoolDown = 2000;
	maxSpeed = 10;
	maxMoveForce = 5;
	turnAngle = 0.0;
	walkSpeed = 5;

}

void Player::Update(float dt) {

	CheckCoolDowns();
	CheckInput();
	btTransform trans;
	playerRigidBody->getMotionState()->getWorldTransform(trans);
	playerPos = vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
	entity.position = playerPos;
	entity.Update(dt);
	//cout << entity.position.x << " " << entity.position.y << " " << entity.position.z << endl;
}


void Player::Render() {
	MasterRenderer::GetInstance().processEntity(&entity);
};

void Player::CheckInput()
{

	float horizontalMovement = 0, verticalMovement = 0;
	int newRot = 0;

	if (InputManager::GetInstance().IsKeyDown(SDLK_w)) {
		verticalMovement = 1;
	};

	if (InputManager::GetInstance().IsKeyDown(SDLK_a)) {
		//horizontalMovement = -1;
		newRot = 2;
	};

	if (InputManager::GetInstance().IsKeyDown(SDLK_s)) {
		verticalMovement = -1;
	};

	if (InputManager::GetInstance().IsKeyDown(SDLK_d)) {
		//horizontalMovement = 1;
		newRot = -2;
	};
	if ((InputManager::GetInstance().IsKeyDown(SDLK_SPACE)) && canJump == true) {
		PlayerJump();
		canJump = false;
	}

	if (InputManager::GetInstance().IsKeyDown(SDLK_LSHIFT)) {
		entity.ChangeAnimation("Run", true);
	}

	PlayerMove(horizontalMovement, verticalMovement, newRot);
}

void Player::PlayerMove(float horizontalInput, float verticalInput, int newRot) {

	//btTransform transform;
	//playerRigidBody->getMotionState()->getWorldTransform(transform);
	//turnAngle -= 1;
	//transform.setRotation(btQuaternion(btVector3(0, 1, 0), turnAngle));

	//playerPos = vec3(playerPos.x, playerPos.y, playerPos.z - 0.001);
	//playerRot = vec3(playerRot.x, playerRot.y + 5, playerRot.z);
	


	//float xForce = horizontalInput * maxMoveForce;
	float zForce =  verticalInput * maxMoveForce;

	vec3 forward = glm::rotate( vec3(0, 0, 1), radians(getPlayerRot().z), vec3(0,1,0));
	forward = normalize(forward);

	btVector3 finalForce = btVector3(forward.x * zForce, playerRigidBody->getLinearVelocity().getY() +  forward.y, forward.z * zForce);

	playerRigidBody->setLinearVelocity(finalForce);
	ControlSpeed();

	entity.position = playerPos;
	entity.rotation = playerRot;

	playerRot = vec3(playerRot.x, playerRot.y, playerRot.z + newRot);

	AnimationController();

};

void Player::PlayerJump() {

	playerRigidBody->applyCentralImpulse(btVector3(0, 8, 0));
	jumpTimeStamp = SDL_GetTicks();
}

void Player::CheckCoolDowns() {

	if (SDL_GetTicks() > jumpTimeStamp + jumpCoolDown) {
		canJump = true;
	}
}

Player* Player::getInstance() {

	if (!instance) {
		instance = new Player();
	}

	return instance;
}

void Player::ControlSpeed() {

	float playerSpeedX = playerRigidBody->getLinearVelocity().getX();
	float playerSpeedY = playerRigidBody->getLinearVelocity().getY();
	float playerSpeedZ = playerRigidBody->getLinearVelocity().getZ();

	if (playerSpeedX > maxSpeed) { playerSpeedX = maxSpeed; };
	if (playerSpeedY > maxSpeed) { playerSpeedY = maxSpeed; };
	if (playerSpeedZ > maxSpeed) { playerSpeedZ = maxSpeed; };

	playerRigidBody->setLinearVelocity(btVector3(playerSpeedX, playerSpeedY, playerSpeedZ));

};

void Player::AnimationController() {


	if ((playerRigidBody->getLinearVelocity().length() > 0) && (playerRigidBody->getLinearVelocity().length() < 5)) {

		if (entity.currentAnimation != "Walk") {
			entity.ChangeAnimation("Walk", true);
		} 
	}

	if ((playerRigidBody->getLinearVelocity().length() > walkSpeed) && (entity.currentAnimation != "Run")) {

		entity.ChangeAnimation("Run", true);

	}
};