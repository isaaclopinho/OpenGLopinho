#include "Enemy.h"
#include "Player.h"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/vector_angle.hpp>

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
    if (PlayerNearby() == true){
        btVector3 position = Maths::glmToBullet(glm::lerp(Maths::bulletToGlm(getWorldPosition()), Maths::bulletToGlm(Player::getInstance()->getWorldPosition()), dt * 1));
        vec3 dir = Maths::bulletToGlm(Player::getInstance()->getWorldPosition() - getWorldPosition());
        
        
        
        float ang = angle(normalize(dir), vec3(0, 0, 1));
        
        if(getWorldPosition().getX() >= Player::getInstance()->getWorldPosition().getX())
            ang = -angle( normalize(dir), vec3(0, 0, 1));
        
            entity->rotation = vec3(entity->rotation.x, entity->rotation.y, degrees(ang));
        
        btVector3 rotation = (Player::getInstance()->getWorldPosition() - getWorldPosition()).normalized();
        printf("%f, %f, %f\n", rotation.x(), rotation.y(), rotation.z());
//        entity->rotation = Maths::bulletToGlm(rotation);
//        entity->rotation.x -= 90;
        if (entity->currentAnimation != "Walk"){
            entity->ChangeAnimation("Walk", true);
        }
        
        this->setPosition(position);
    }
    
}

bool Enemy::PlayerNearby(){
    btScalar dist = (Player::getInstance()->getWorldPosition() - getWorldPosition()).norm();
    bool neara = (dist <= 100);
    if (neara) printf("player por perto\n");
    else printf("player nÃo por perto \n");
    return neara;
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
