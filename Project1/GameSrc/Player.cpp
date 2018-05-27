#include "Player.h"

Player::Player() : entity(Loader::LoadModel("res/Models/hans.dae"), playerPos, playerRot, 1, "", false)
{
	 

}

void Player::Update(float dt) {

	CheckInput();
}


void Player::Render() {
	MasterRenderer::GetInstance().processEntity(&entity);
};

void Player::CheckInput()
{

	if (InputManager::GetInstance().IsKeyDown(SDLK_a)) {
		PlayerMove();
	};
}

void Player::PlayerMove() {
	
	playerPos = vec3(playerPos.x, playerPos.y, playerPos.z - 0.001);
	playerRot = vec3(playerRot.x, playerRot.y + 1, playerRot.z);
	entity.position = playerPos;
	entity.rotation = playerRot;
};