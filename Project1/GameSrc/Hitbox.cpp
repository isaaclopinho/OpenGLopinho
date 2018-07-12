//
//  Hitbox.cpp
//  OpenGLopinho2
//
//  Created by Thiago Dantas on 08/07/18.
//  Copyright © 2018 Thiago Dantas. All rights reserved.
//

#include <stdio.h>
#include "Hitbox.h"


//PhysicsObject(mass, shape, position, rotation, btVector3 scale, btVector3 inercia, Entity* e = NULL);
Hitbox::Hitbox(btVector3 position, btVector3 rotation, btVector3 scale, Entity* e): PhysicsObject(0,Box, position, rotation, scale, btVector3(0,0,0), e){
    entity = e;
    toggleContact(false);// desativa contato
    type = "Trigger"; // tag = trigger;
    dist = 12.0f; //distancia a frente que a hitbox deve ficar;
}

void Hitbox::Update(float dt){
    if (owner != NULL ){
        if (!owner->Remove()){
            btVector3 ownerPos = owner->getWorldPosition();
            ownerPos.setY(ownerPos.getY()+5);
            setPosition(Maths::glmToBullet(owner->getForwardVector()) * dist + ownerPos);
            PhysicsObject::Update(dt);
        } else {
            remove = true;
        }
    }
}
