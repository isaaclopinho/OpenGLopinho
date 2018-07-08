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

enum PhysicsBitMask{
    COL_NOTHING = 0,
    COL_PLAYER = 1<<1, //primeiro bit ligado
    COL_ENEMY = 1<<2, //segundo bit ligado
    COL_WALL = 1<<3, //terceiro bit ligado...
    COL_FLOOR = 1<<4
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
    PhysicsObject(float mass, PhysicsShape shape, btVector3 inercia, Entity* e);
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
    void SetRotation(btVector3 rotation);

    glm::vec3 getForwardVector();
    glm::vec3 getRot() { return entity->rotation; };
    glm::vec3 getPos() { return entity->position; };
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
