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
#include "../Movie.h"

Boss::Boss(float mass, PhysicsShape shape, btVector3 inercia, Entity* e): PhysicsObject(mass, shape, inercia, e), cd(5){
    hp = 1000;
    maxHP = 1000;
    getPhysicsBody()->forceActivationState(DISABLE_DEACTIVATION);
    type = "Boss";

	limitX = Player::getInstance()->limitX;
	limitZ = Player::getInstance()->limitZ;

}

Boss::~Boss(){
    
}

void Boss::RecieveDamage(int damage){
    hp -= damage;
    if(hp <= 0){
		morto = true;
    }
}

void Boss::Update(float dt){
    if (entity != NULL) {
        entity->Update(dt);
        entity->position = Maths::bulletToGlm(getWorldPosition());
        entity->position.y += getScale().getY()/4;
    }
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

int Boss::GetHP(){
    return hp;
}

int Boss::GetMaxHP(){
    return maxHP;
}
