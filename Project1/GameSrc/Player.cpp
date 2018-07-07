#include "Player.h"
#include <glm/gtx/rotate_vector.hpp>
Player* Player::instance = 0;
//(mass, shape, position, rotation, scale, inercia, entity);
Player::Player() : entity(Loader::LoadModel("res/Models/hans_mesh2.dae"), playerPos, playerRot, vec3(1, 1, 1), "Walk", true), PhysicsObject(100, PhysicsShape::Capsule, btVector3(0,10,0), btVector3(-90, 0, 0), btVector3(2,1.5,0), btVector3(), &entity)
{
	//Initialize Player Variables

	canJump = true;
	jumpTimeStamp = 0;
	jumpCoolDown = 2000;
	maxSpeed = 50;
	maxMoveForce = 5;
	turnAngle = 0.0;
	walkSpeed = 15;

	maxVelocity = 50;
	velocity = 0;
	minVelocity = 0;
	velocityStep = 0.5f;

    getPhysicsBody()->forceActivationState(DISABLE_DEACTIVATION);
    maxHP = hp = 100;
    type = "Player";
}

void Player::Update(float dt) {

	CheckCoolDowns();
	CheckInput();
	btTransform trans = getWorldTransForm();
	entity.position = Maths::bulletToGlm(getWorldPosition());
	//entity.rotation = Maths::bulletToGlm(getWorldRotation());
	//entity.rotation.x -= 90;
	entity.Update(dt);
	//cout << entity.position.x << " " << entity.position.y << " " << entity.position.z << endl;
	//cout << "vel X: " << getVelocity().getX() << "vel Y: " << getVelocity().getY() << "vel Z: " << getVelocity().getZ() <<  endl;
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

	//cout << playerRigidBody->getLinearVelocity().length() << endl;

	PlayerMove(horizontalMovement, verticalMovement, newRot);

	//ControlSpeed();
	AnimationController();
}

void Player::PlayerMove(float horizontalInput, float verticalInput, int newRot) {

	//btTransform transform;
	//playerRigidBody->getMotionState()->getWorldTransform(transform);
	//turnAngle -= 1;
	//transform.setRotation(btQuaternion(btVector3(0, 1, 0), turnAngle));

	//playerPos = vec3(playerPos.x, playerPos.y, playerPos.z - 0.001);
	//playerRot = vec3(playerRot.x, playerRot.y + 5, playerRot.z);
	


	//float xForce = horizontalInput * maxMoveForce;

	if (verticalInput == 1) {
		velocity = glm::clamp(velocity+velocityStep, minVelocity, maxVelocity);
	}
	else if (verticalInput == -1) {
		velocity = glm::clamp(velocity - velocityStep, -maxVelocity/2, maxVelocity);
	}
	else {

		if(velocity < 0)
			velocity = glm::clamp(velocity + velocityStep*2, -maxVelocity, minVelocity);
		else
			velocity = glm::clamp(velocity - velocityStep*2, minVelocity, maxVelocity);

	}

	//std::cout << velocity << endl;

	float zForce = velocity;

	vec3 forward = glm::rotate( vec3(0, 0, 1), radians(getPlayerRot().z), vec3(0,1,0));
	forward = normalize(forward);

	btVector3 finalForce = btVector3(forward.x * zForce, getVelocity().getY() +  forward.y, forward.z * zForce);

    setVelocity(finalForce);
    
	entity.position = playerPos;
	entity.rotation = playerRot;

	playerRot = vec3(playerRot.x, playerRot.y, playerRot.z + newRot);



};

vec3 Player::getForwardVector() {
	vec3 forward = glm::rotate(vec3(0, 0, 1), radians(getPlayerRot().z), vec3(0, 1, 0));
	forward = normalize(forward);
	return forward;
};

void Player::PlayerJump() {
    
    applyForce(btVector3(0,5500,0));
//    _body->applyCentralImpulse(btVector3(0, 1000, 0));
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

	float playerSpeedX = getVelocity().getX();
	float playerSpeedY = getVelocity().getY();
	float playerSpeedZ = getVelocity().getZ();

	if (playerSpeedX > maxSpeed) { playerSpeedX = maxSpeed; };
	if (playerSpeedY > maxSpeed) { playerSpeedY = maxSpeed; };
	if (playerSpeedZ > maxSpeed) { playerSpeedZ = maxSpeed; };
	if (playerSpeedX < -maxSpeed) { playerSpeedX = -maxSpeed; };
	if (playerSpeedY < -maxSpeed) { playerSpeedY = -maxSpeed; };
	if (playerSpeedZ < -maxSpeed) { playerSpeedZ = -maxSpeed; };

    setVelocity(btVector3(playerSpeedX, playerSpeedY, playerSpeedZ));
};

void Player::AnimationController() {


	if ((velocity > 0) && (velocity < walkSpeed)) {

		if (entity.currentAnimation != "Walk") {
			entity.ChangeAnimation("Walk", true);
		} 
	}

	if (velocity < 0) {
		if (entity.currentAnimation != "Walk") {
			entity.ChangeAnimation("Walk", true);
		}
	}

	if ((velocity > walkSpeed) && (entity.currentAnimation != "Run")) {

		entity.ChangeAnimation("Run", true);

	}

};

void Player::SetHP(int newHP){
    hp = newHP;
}
int Player::GetHP(){
    return hp;
}
void Player::SetMaxHP(int newMaxHP){
    maxHP = newMaxHP;
}
int Player::GetMaxHP(){
    return maxHP;
}
void Player::LoseHP(int hpLoss){
    hp -= hpLoss;
    if(hp <= 0){
        hp = 0;
        cout << "DEAD" << endl;
    }
}
