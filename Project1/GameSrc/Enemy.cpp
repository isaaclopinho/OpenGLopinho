#include "Enemy.h"
#include "Player.h"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/vector_angle.hpp>

#define ATTCD 2

Enemy::Enemy(float mass, PhysicsShape shape, btVector3 inercia, Entity* e): PhysicsObject(mass, shape, inercia, e), invulneravel(1), walkCD(2), attCD(2), atkAnimation(0)
{
    
    getPhysicsBody()->forceActivationState(DISABLE_DEACTIVATION);
    type = "Enemy";
    health = 1;
    
	limitX = Player::getInstance()->limitX;
	limitZ = Player::getInstance()->limitZ;

    currentState = EnemyStates::IDLE;
}
Enemy::~Enemy(){
    
}

void Enemy::Update(float dt){
    PhysicsObject::Update(dt);
    invulneravel.Update(dt);
    atkAnimation.Update(dt);
    
    if(!atkAnimation.IsInCooldown()){
        if (entity->currentAnimation == "AttackDuplo") {
            entity->ChangeAnimation("IdleRight", true);
        }
    }
    
    switch (currentState) {
        case EnemyStates::IDLE:
            if (PlayerNearby() == true) {
                currentState = EnemyStates::WALKING;
                attCD.SetCooldown(ATTCD);
                attCD.Reset();
            }
            break;
        case EnemyStates::WALKING:
            if (PlayerNearby() == false) {
                currentState = EnemyStates::IDLE;
                walkCD.SetCooldown(0);
            }
            else if(!walkCD.IsInCooldown()){
                attCD.Update(dt);
//                if(!atkAnimation.IsInCooldown()){
//                     if (entity->currentAnimation != "IdleRight") {
//                         entity->ChangeAnimation("IdleRight", true);
//                         cout << "alo" << endl;
//                     }
//
//                }
//                else{
//                    atkAnimation.Update(dt);
//                }
                if (!attCD.IsInCooldown()) {
                    currentState = EnemyStates::ATTACKING;
                    attCD.SetCooldown(1);
                    attCD.Reset();
                }
                else {
                    btVector3 position = Maths::glmToBullet(glm::lerp(Maths::bulletToGlm(getWorldPosition()), Maths::bulletToGlm(Player::getInstance()->getWorldPosition()), dt * 1));
                    vec3 dir = Maths::bulletToGlm(Player::getInstance()->getWorldPosition() - getWorldPosition());
                    
                    float ang = angle(normalize(dir), vec3(0, 0, 1));
                    
                    if (getWorldPosition().getX() >= Player::getInstance()->getWorldPosition().getX())
                        ang = -angle(normalize(dir), vec3(0, 0, 1));
                    
                    entity->rotation = vec3(entity->rotation.x, entity->rotation.y, degrees(ang));
                    
                    btVector3 rotation = (Player::getInstance()->getWorldPosition() - getWorldPosition()).normalized();
//                    printf("%f, %f, %f\n", rotation.x(), rotation.y(), rotation.z());
                    //        entity->rotation = Maths::bulletToGlm(rotation);
                    //        entity->rotation.x -= 90;
                    if (entity->currentAnimation != "Walk") {
                        entity->ChangeAnimation("Walk", true);
                    }
                    
                    this->setPosition(position);
                }
            }
            if (walkCD.IsInCooldown()){
                walkCD.Update(dt);
            }
            break;
        case EnemyStates::ATTACKING:
            
            attCD.Update(dt);
            if (!attCD.IsInCooldown()) {
                entity->ChangeAnimation("AttackDuplo", false);
                atkAnimation.SetCooldown(entity->mesh->animationMap["AttackDuplo"]->mDuration * entity->mesh->animationMap["AttackDuplo"]->mTicksPerSecond);
                //                    cout << "Valor: " << entity->mesh->animationMap["AttackDuplo"]->mDuration * entity->mesh->animationMap["AttackDuplo"]->mTicksPerSecond << endl;
                atkAnimation.Reset();
                btVector3 dir = (Player::getInstance()->getWorldPosition() - getWorldPosition()).normalized();
                applyForce(dir * 2000);
                walkCD.SetCooldown(2);
                currentState = EnemyStates::WALKING;
                attCD.SetCooldown(3);
            }
            //currentState = EnemyStates::IDLE;
            /*
             CHANGE ANIMATION AND MAKE ENEMY CHARGE AT PLAYER
             */
    }




	if (limitZ != vec2(0, 0) && limitX != vec2(0, 0)) {

		if (getWorldPosition().z() <= limitZ.x)
			setPosition(btVector3(getWorldPosition().x(), getWorldPosition().y(), limitZ.x));


		if (getWorldPosition().z() >= limitZ.y)
			setPosition(btVector3(getWorldPosition().x(), getWorldPosition().y(), limitZ.y));

		if (getWorldPosition().x() <= limitX.x)
			setPosition(btVector3(limitX.x, getWorldPosition().y(), getWorldPosition().z()));


		if (getWorldPosition().x() >= limitX.y)
			setPosition(btVector3(limitX.y, getWorldPosition().y(), getWorldPosition().z()));

	}
}

bool Enemy::PlayerNearby(){
    btScalar dist = (Player::getInstance()->getWorldPosition() - getWorldPosition()).norm();
    bool neara = (dist <= 100);
//    if (neara) printf("player por perto\n");
//    else printf("player nÃo por perto \n");
    return neara;
}

void Enemy::RecieveDamage(int damage){
    if (!invulneravel.IsInCooldown()){
        health-=damage;
        if (health > 0 ) {
            Player* p = Player::getInstance();
            printf("apanhou\n");
            btVector3 direcao = (getWorldPosition() - p->getWorldPosition()).normalized();
            
//            cout << "direcao: " << direcao.x() << " " << direcao.y() << " " << direcao.z() << endl;
            
            direcao *= btVector3(1, 4, 1);
            
            applyForce(direcao * 3000);
            invulneravel.Reset(); //start();
        } else {
            this->remove = true;
        }
    }
}
