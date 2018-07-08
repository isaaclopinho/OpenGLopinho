#include "Enemy.h"

Enemy::Enemy(float mass, PhysicsShape shape, btVector3 inercia, Entity* e): PhysicsObject(mass, shape, inercia, e){
    
    getPhysicsBody()->forceActivationState(DISABLE_DEACTIVATION);
    type = "Enemy";
    health = 100;
    
    
}
Enemy::~Enemy(){
    
}

void Enemy::RecieveDamage(int damage){
    health-=damage;
    if (health > 0 ) {
        printf("apanhou\n");
    } else {
        this->remove = true;
    }
}
