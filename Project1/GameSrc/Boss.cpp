//
//  Boss.cpp
//  OpenGLopinho2
//
//  Created by Gabriel Mota Araújo on 08/07/18.
//  Copyright © 2018 Gabriel Mota Araújo. All rights reserved.
//

#include <iostream>
#include "Boss.h"
#include "Player.h"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/vector_angle.hpp>


Boss::Boss(float mass, PhysicsShape shape, btVector3 inercia, Entity* e): PhysicsObject(mass, shape, inercia, e), cd(5){
    hp = 500;
    maxHP = 1000;
    getPhysicsBody()->forceActivationState(DISABLE_DEACTIVATION);
    type = "Enemy";
}

Boss::~Boss(){
    
}

void Boss::RecieveDamage(int damage){
    hp -= damage;
    if(hp <= 0){
        cout << "BOSS MORREU" << endl;
        this->remove = true;
    }
}

void Boss::Update(float dt){
    PhysicsObject::Update(dt);
    cd.Update(dt);
    btVector3 position = Maths::glmToBullet(glm::lerp(Maths::bulletToGlm(getWorldPosition()), Maths::bulletToGlm(Player::getInstance()->getWorldPosition()), dt * 1));
    vec3 dir = Maths::bulletToGlm(Player::getInstance()->getWorldPosition() - getWorldPosition());
    
    float ang = angle(normalize(dir), vec3(0, 0, 1));
    
    if (getWorldPosition().getX() >= Player::getInstance()->getWorldPosition().getX())
        ang = -angle(normalize(dir), vec3(0, 0, 1));
    
    entity->rotation = vec3(entity->rotation.x, entity->rotation.y, degrees(ang));
    
    btVector3 rotation = (Player::getInstance()->getWorldPosition() - getWorldPosition()).normalized();
    
    if(!cd.IsInCooldown()){
        atira = true;
        cd.Reset();
    }
    else{
        atira = false;
    }
    
}

int Boss::GetHP(){
    return hp;
}

int Boss::GetMaxHP(){
    return maxHP;
}
