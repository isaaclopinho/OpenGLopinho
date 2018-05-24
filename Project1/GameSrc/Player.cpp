#include "Player.h"

Player::Player() : entity(Loader::LoadModel("res/Models/hans.obj"), vec3(0, 0, -5), vec3(0, 0, 0), 1, "", false)
{
	 

}


void Player::Render() {
	MasterRenderer::GetInstance().processEntity(&entity);
};