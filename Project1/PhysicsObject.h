#pragma once

#include "GameObject.h"
#include "Entity.h"
#include <iostream>
using namespace std;

enum PhysicsShape{
    Box,
    Capsule,
    Sphere,
    Cyllinder
};

class PhysicsObject : public GameObject {
	
public:

    Entity * entity;
    PhysicsObject() {}; // necessário?
//    PhysicsObject(btCollisionShape* collider, vec3 objectPos, btScalar objectMass); //old constructor
//    float mass, PhysicsShape shape, btVector3 position, btVector3 rotation,btVector3 scale, btVector3 inercia, Entity* mesh)
    PhysicsObject(float mass, PhysicsShape shape, btVector3 position, btVector3 rotation,
                  btVector3 scale, btVector3 inercia, Entity* mesh = NULL);
    ~PhysicsObject();
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    
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
