#include "Player.h"

Player* Player::instance = 0;

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
	
	//Initialize Player Variables
	canJump = true;
	jumpTimeStamp = 0;
	jumpCoolDown = 2000;
	maxSpeed = 5;
	maxMoveForce = 20;
	turnAngle = 0.0;

}

void Player::Update(float dt) {

	CheckCoolDowns();
	CheckInput();
	btTransform trans;
	playerRigidBody->getMotionState()->getWorldTransform(trans);
	playerPos = vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
	entity.position = playerPos;
	//entity.Update(dt);
	//cout << playerPos.z << endl;
}


void Player::Render() {
	MasterRenderer::GetInstance().processEntity(&entity);
};

void Player::CheckInput()
{
	float horizontalMovement = 0, verticalMovement = 0;

	if (InputManager::GetInstance().IsKeyDown(SDLK_w)) {
		verticalMovement = 1;
	};

	if (InputManager::GetInstance().IsKeyDown(SDLK_a)) {
		horizontalMovement = -1;
	};

	if (InputManager::GetInstance().IsKeyDown(SDLK_s)) {
		verticalMovement = -1;
	};

	if (InputManager::GetInstance().IsKeyDown(SDLK_d)) {
		horizontalMovement = 1;
	};
	if ((InputManager::GetInstance().IsKeyDown(SDLK_SPACE)) && canJump == true) {
		PlayerJump();
		canJump = false;
	}

	PlayerMove(horizontalMovement, verticalMovement);
}

void Player::PlayerMove(float horizontalInput, float verticalInput) {

	//btTransform transform;
	//playerRigidBody->getMotionState()->getWorldTransform(transform);
	//turnAngle -= 1;
	//transform.setRotation(btQuaternion(btVector3(0, 1, 0), turnAngle));

	//playerPos = vec3(playerPos.x, playerPos.y, playerPos.z - 0.001);
	//playerRot = vec3(playerRot.x, playerRot.y + 5, playerRot.z);
	//entity.position = playerPos;
	//entity.rotation = playerRot;



	float xForce = horizontalInput * maxMoveForce;
	float zForce = verticalInput * maxMoveForce;

	btVector3 finalForce = btVector3(xForce, 0, zForce);
	playerRigidBody->applyCentralImpulse(finalForce);

	ControlSpeed();


	//btVector3 newLinearVelocity = btVector3(playerRigidBody->getLinearVelocity().getX(), playerRigidBody->getLinearVelocity().getY(), playerRigidBody->getLinearVelocity().getZ() + (speed /** Game::GetInstance()->GetDeltaTime()*/));
	//playerRigidBody->setLinearVelocity(newLinearVelocity);
	//speed = playerRigidBody->getLinearVelocity().getZ();

	

};

void Player::PlayerJump() {

	playerRigidBody->applyCentralImpulse(btVector3(0, 400, 0));
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
