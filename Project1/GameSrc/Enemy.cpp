#include "Enemy.h"
#include "../PhysicsObject.h"

void Enemy::Update(float dt){
	btTransform trans;
	getPhysicsBody()->getMotionState()->getWorldTransform(trans);
	entity->position = Maths::bulletToGlm(btVector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));

	CheckHealth();


}

void Enemy::CheckHealth()
{
	if (health <= 0) { isAlive = false; };
}
