#include "Player.h"
#include <glm/gtx/rotate_vector.hpp>
Player* Player::instance = 0;
//(mass, shape, position, rotation, scale, inercia, entity);

Player::Player() : entity(Loader::LoadModel("res/Models/hans.dae"), playerPos, playerRot, vec3(1, 1, 1), "Walk", true), PhysicsObject(100, PhysicsShape::Box, btVector3(0,1, -700), btVector3(-90, 0, 0), btVector3(4,6,4), btVector3(), &entity), jump(0.2), invulneravel(1), ataque(1), knockback(0.5), dash(0.5)
{
	//Initialize Player Variables

	canJump = true;
	maxSpeed = 50;
	maxMoveForce = 5;
	turnAngle = 0.0;
	walkSpeed = 45;

	maxVelocity = 50;
	velocity = 0;
	minVelocity = 0;
	velocityStep = 0.5f;
    
    getPhysicsBody()->setFriction(0.8);
	getPhysicsBody()->setAngularFactor(btVector3(0, 0, 0));

    getPhysicsBody()->forceActivationState(DISABLE_DEACTIVATION);
    maxHP = hp = 100;
    type = "Player";

	for (int i = 0; i < 10; ++i) {
		char file[100];
		sprintf(file, "res/sounds/hansScreams/scream%d.wav", i + 1);
		attackScreams[i] = make_unique<AudioSource>(file, false, false);
	}
	for (int i = 0; i < 12; ++i) {
		char file[100];
		sprintf(file, "res/sounds/sword/Espada %d.wav", i + 1);
		swordSounds[i] = make_unique<AudioSource>(file, false, true);
	}
	elapsedTime = 0;
	playSwordSound = false;
}

void Player::land(){
    //respeitar cooldown pq fisica atualiza mto rapido antes de pular, land é chamado :((
    if (jump.IsInCooldown() == false ) canJump = true; //tr00;
}

void Player::Update(float dt) {

	CheckInput();
	btTransform trans = getWorldTransForm();
	entity.position = Maths::bulletToGlm(getWorldPosition());
//    entity.position.y -= (getScale().getX()/2 + getScale().getY()/2); //ajusta a posição do player baseado no scale
    entity.position.y -= 7.5;
	//entity.rotation = Maths::bulletToGlm(getWorldRotation());
	//entity.rotation.x -= 90;
	entity.Update(2*dt);
    jump.Update(dt);
    ataque.Update(dt);
    invulneravel.Update(dt);
    knockback.Update(dt);
    dash.Update(dt);

	AnimationController(dt);
    
    if(limitZ != vec2(0,0) && limitX != vec2(0,0)){

        if (getWorldPosition().z() <= limitZ.x)
            setPosition(btVector3(getWorldPosition().x(), getWorldPosition().y(), limitZ.x));
        
        
        if (getWorldPosition().z() >= limitZ.y)
            setPosition(btVector3(getWorldPosition().x(), getWorldPosition().y(), limitZ.y));
        
        if (getWorldPosition().x() <= limitX.x)
            setPosition(btVector3(limitX.x, getWorldPosition().y(), getWorldPosition().z()));
        
        
        if (getWorldPosition().x() >= limitX.y)
            setPosition(btVector3(limitX.y, getWorldPosition().y(), getWorldPosition().z()));
        
    }

	if (playSwordSound) {
		elapsedTime += dt;
		if (elapsedTime > 0.5) {
			swordSounds[rand() % 12]->Play();
			playSwordSound = false;
			elapsedTime = 0;
		}
	}
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
	if ((InputManager::GetInstance().KeyPress(SDLK_SPACE)) && canJump) {
        jump.Reset(); //start
        PlayerJump();
    }
    if ((InputManager::GetInstance().ControllerButtonPress(X360_X)||(InputManager::GetInstance().KeyPress(SDLK_k)))){
		if (!atacou) {
			atacou = true;
			attackScreams[rand() % 10]->Play();
			playSwordSound = true;
		}
    }

	//cout << playerRigidBody->getLinearVelocity().length() << endl;

	PlayerMove(horizontalMovement, verticalMovement, newRot);

	//ControlSpeed();
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

    
    
    if(!knockback.IsInCooldown() && !dash.IsInCooldown()){
        setVelocity(finalForce);
        entity.position = playerPos;
        entity.rotation = playerRot;
        playerRot = vec3(playerRot.x, playerRot.y, playerRot.z + newRot);
    }
    else{
        cout << "aqui" << endl;
    }



};

vec3 Player::getForwardVector() {
	vec3 forward = glm::rotate(vec3(0, 0, 1), radians(getPlayerRot().z), vec3(0, 1, 0));
	forward = normalize(forward);
	return forward;
};

void Player::PlayerJump() {
    
    applyForce(btVector3(0,6500,0));
    canJump = false;
//    _body->applyCentralImpulse(btVector3(0, 1000, 0));
//    jumpTimeStamp = SDL_GetTicks();
}

void Player::CheckCoolDowns() {

//    if (SDL_GetTicks() > jumpTimeStamp + jumpCoolDown) {
//        canJump = true;
//    }
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

void Player::AnimationController(float dt) {


	if (atacou) {
		timeAtack += dt;

		if (timeAtack >= 1) {
			timeAtack = 0;
			atacou = false;
		}

		if (entity.currentAnimation != "AtackDouble") {
			entity.ChangeAnimation("AtackDouble", true);
		}
	}
	else {



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

		if (velocity == 0) {
			if (entity.currentAnimation != "Idle") {
				entity.ChangeAnimation("Idle", true);
			}
		}

		if ((velocity > walkSpeed) && (entity.currentAnimation != "Run")) {

			entity.ChangeAnimation("Run", true);

		}
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
void Player::LoseHP(int hpLoss, btVector3 origin){
    
    if (invulneravel.IsInCooldown() == false){
        
        knockback.Reset();
        
        // onde o inimigo está - onde o player está = vetor que aponta do inimigo ao player;
        btVector3 direction = (getWorldPosition() - origin).normalized();
        printf("dir: %f, %f, %f \n", direction.x(), direction.y(), direction.z());
        direction.setY(abs(direction.y()));
        direction.setX(direction.x());
        direction.setZ(direction.z());
        applyForce((direction) * 5000);
        this->invulneravel.Reset(); //start
        hp -= hpLoss;
        if(hp <= 0){
            hp = 0;
            cout << "DEAD" << endl;
        }
    }
    
}

void Player::Dash(){
    if(!dash.IsInCooldown()){
        applyForce(Maths::glmToBullet(getForwardVector() * 10000.0f));
        std::cout << "forward: " << getForwardVector().x << " " << getForwardVector().y << " " << getForwardVector().z << endl;
        dash.Reset();
    }
}
