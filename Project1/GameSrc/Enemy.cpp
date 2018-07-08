#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(float mass, PhysicsShape shape, btVector3 inercia, Entity* e): PhysicsObject(mass, shape, inercia, e), invulneravel(1){
    
    getPhysicsBody()->forceActivationState(DISABLE_DEACTIVATION);
    type = "Enemy";
    health = 100;
    
    
}
Enemy::~Enemy(){
    
}

void Enemy::Update(float dt){
    PhysicsObject::Update(dt);
    invulneravel.Update(dt);
}

void Enemy::RecieveDamage(int damage){
    if (!invulneravel.IsInCooldown()){
        health-=damage;
        if (health > 0 ) {
            Player* p = Player::getInstance();
            printf("apanhou\n");
            btVector3 direcao = (getWorldPosition() - p->getWorldPosition()).normalized();
            
            cout << "direcao: " << direcao.x() << " " << direcao.y() << " " << direcao.z() << endl;
            
            direcao *= btVector3(1, 4, 1);
            
            applyForce(direcao * 3000);
            invulneravel.Reset(); //start();
        } else {
            this->remove = true;
        }
    }
}
