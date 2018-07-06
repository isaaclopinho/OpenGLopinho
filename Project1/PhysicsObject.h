#pragma once

#include "GameObject.h"
#include "MasterRenderer.h"
#include "Entity.h"
#include <iostream>
using namespace std;

enum PhysicsShape{
    Box,
    Capsule,
    Sphere,
    Cyllinder
};

/*enum CollisionFlags
{
    CF_STATIC_OBJECT= 1, // objeto statico
    CF_KINEMATIC_OBJECT= 2,
    CF_NO_CONTACT_RESPONSE = 4, //objeto não colide mas age como trigger?
    CF_CUSTOM_MATERIAL_CALLBACK = 8, //objeto com essa flag chama função custom de colisão.
    CF_CHARACTER_OBJECT = 16,
    CF_DISABLE_VISUALIZE_OBJECT = 32, //disable debug drawing
    CF_DISABLE_SPU_COLLISION_PROCESSING = 64,//disable parallel/SPU processing
    CF_HAS_CONTACT_STIFFNESS_DAMPING = 128,
    CF_HAS_CUSTOM_DEBUG_RENDERING_COLOR = 256,
    CF_HAS_FRICTION_ANCHOR = 512,
    CF_HAS_COLLISION_SOUND_TRIGGER = 1024
};*/

class PhysicsObject : public GameObject {
	
public:

    Entity* entity;
//    PhysicsObject() {}; // necessário?
    PhysicsObject(float mass, PhysicsShape shape, btVector3 position, btVector3 rotation,
                  btVector3 scale, btVector3 inercia, Entity* e = NULL);
    
    ~PhysicsObject();
    virtual void Update(float dt);
    virtual void Render();
    
    btVector3 getWorldPosition();
    btVector3 getWorldRotation();
    btTransform getWorldTransForm();
    btRigidBody* getPhysicsBody() {return _body;};
    void applyForce(btVector3 direction);
    void setPosition(btVector3 position);
    void translate(btVector3 position);
    void setVelocity(btVector3 velocity);
    btVector3 getVelocity();
    void toggleGravity(bool flag);
    void toggleContact(bool flag);
    

private:
    
    btRigidBody* _body;
    btCollisionShape* _shape;
    btVector3 _position;
    btQuaternion _rotation;
    btDefaultMotionState* _motionState;
    
    btScalar _mass;
    btVector3 _inercia;
	void CheckInput();
	void PlayerMove();
};
