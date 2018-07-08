//
//  Hitbox.h
//  OpenGLopinho2
//
//  Created by Thiago Dantas on 08/07/18.
//  Copyright © 2018 Thiago Dantas. All rights reserved.
//
#include "../PhysicsObject.h"


#ifndef Hitbox_h
#define Hitbox_h
class Hitbox: public PhysicsObject{
public:
    PhysicsObject* owner;
    Hitbox(btVector3 position, btVector3 rotation, btVector3 scale, Entity* e = NULL);
    ~Hitbox() {};
    virtual void Update(float dt);
    float dist;
private:
    
};



#endif /* Hitbox_h */
