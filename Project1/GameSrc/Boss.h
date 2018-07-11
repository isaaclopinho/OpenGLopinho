//
//  Boss.h
//  OpenGLopinho2
//
//  Created by Gabriel Mota Araújo on 08/07/18.
//  Copyright © 2018 Gabriel Mota Araújo. All rights reserved.
//

#include "../PhysicsObject.h"
#include "../Timer.h"

class Boss: public PhysicsObject{
    
    enum BossStates {
        IDLE,
        ATTACKING
    };
    
    BossStates currentState;
    
public:
    Boss(float mass, PhysicsShape shape, btVector3 inercia, Entity* e);
    ~Boss();
    void RecieveDamage(int damage);
    virtual void Update(float dt);
    bool atira = false;
    int GetHP();
    int GetMaxHP();

	vec2 limitX = vec2();
	vec2 limitZ = vec2();
    
	bool morto=false;
private:
    int hp, maxHP;
    Timer cd;
};
