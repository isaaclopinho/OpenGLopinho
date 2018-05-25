#include "Player.h"

Player::Player() : entity(Loader::LoadModel("res/Models/hans.dae"), playerPos, vec3(0, 0, 0), 1, "", false)
{
	 

}

void Player::Update(float dt) {

	PlayerMove();
}


void Player::Render() {
	MasterRenderer::GetInstance().processEntity(&entity);
};

void Player::PlayerMove() {
	
	playerPos = vec3(playerPos.x, playerPos.y, playerPos.z - 0.001);
	entity.position = playerPos;
	cout << playerPos.z << endl;
};