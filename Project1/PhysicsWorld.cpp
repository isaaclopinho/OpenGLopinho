//
//  PhysicsWorld.cpp
//  OpenGLopinho2
//
//  Created by Thiago Dantas on 04/07/18.
//  Copyright © 2018 Thiago Dantas. All rights reserved.
//

#include "PhysicsWorld.h"
#include "Enemy.h"
#include "Player.h"

#include <stdio.h>

struct YourOwnFilterCallback : public btOverlapFilterCallback
{
    // return true when pairs need collision
    virtual bool    needBroadphaseCollision(btBroadphaseProxy* proxy0,btBroadphaseProxy* proxy1) const
    {
        bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
        collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);
        
        //add some additional logic here that modified 'collides'
        return collides;
    }
};

PhysicsWorld::PhysicsWorld(){
    _broadphase = new btDbvtBroadphase();
    _collisionConfiguration = new btDefaultCollisionConfiguration();
    _dispatcher = new btCollisionDispatcher(_collisionConfiguration);
    _solver = new btSequentialImpulseConstraintSolver();
    _world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfiguration);
    _world->setGravity(btVector3(0, -98, 0));
    _testDidCollided = false;
    _debugDrawer = new BulletDebugDrawer();
    _debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    _world->setDebugDrawer(_debugDrawer);
    btOverlapFilterCallback * filterCallback = new YourOwnFilterCallback();
    _world->getPairCache()->setOverlapFilterCallback(filterCallback);
    
}

PhysicsWorld::~PhysicsWorld(){
    delete _world;
    delete _solver;
    delete _collisionConfiguration;
    delete _dispatcher;
    delete _broadphase;
    delete _debugDrawer;
}


void PhysicsWorld::updateWorld(float dt){
    _world->stepSimulation(dt);
    
    // Numero de contatos possiveis
    int num = _world->getDispatcher()->getNumManifolds();
    
    for (int i = 0; i < num; i++)
    {
        // Contato especifico
        btPersistentManifold* contact = _world->getDispatcher()->getManifoldByIndexInternal(i);
        
        int numCollisions = contact->getNumContacts();
        
        if (numCollisions > 0)
        {
            const btCollisionObject* bodyA = contact->getBody0();
            const btCollisionObject* bodyB = contact->getBody1();
            
            // Faz algo
            _testDidCollided = true;
            // recuperando o objeto com o userpointer
            //antes de fazer isso todos os corpos adicionados tem que ser PhysicsObject;
            PhysicsObject *physicsBodyA = (PhysicsObject*)bodyA->getUserPointer();
            PhysicsObject *physicsBodyB = (PhysicsObject*)bodyB->getUserPointer();

            if(physicsBodyA->Is("Player"))
            {
                if (physicsBodyB->Is("Floor")){
//                    printf("colisão com chão");
                }
                if (physicsBodyB->Is("Enemy")){
                    Enemy *e = (Enemy*)bodyB->getUserPointer();
//                    Player::getInstance();
                }
                
            }else if(physicsBodyB->Is("Player"))
            {
                if (physicsBodyA->Is("Floor")){
                    //                    printf("colisão com chão");
                }
                if (physicsBodyA->Is("Enemy")){
                    Enemy *e = (Enemy*)bodyA->getUserPointer();
                }
            }
            
            if(physicsBodyA->Is("Trigger")){
                if (InputManager::GetInstance().ControllerButtonPress(X360_X)){
                    
                    Enemy *e = (Enemy*)bodyB->getUserPointer();
                    e->RecieveDamage(10);
                }
            } else if(physicsBodyB->Is("Trigger")){
                if (InputManager::GetInstance().ControllerButtonPress(X360_X)){
                    Enemy *e = (Enemy*)bodyA->getUserPointer();
                    e->RecieveDamage(10);
                }
            }
            
            
            
        }
        
    }
    
}

void PhysicsWorld::debugDraw(){
    _world->debugDrawWorld();
}

//! Adiciona Objeto com BitMask << USA ESSE MÉTODO!!!
/*!
 \param obj objeto a adicionar o corpo fisico;
 \param objBit bit que representa o objeto.
 \param collides bits que o objeto colide (tem de ser mútuo para ter colisão).
 */
void PhysicsWorld::addPhysicsObject(PhysicsObject *obj, PhysicsBitMask objBit, int collides){
    _world->addRigidBody(obj->getPhysicsBody(), objBit, collides);
}

//! Adiciona Objeto (BIT MASK = 0);
/*!
 \param obj objeto a adicionar o corpo fisico;
 */
void PhysicsWorld::addPhysicsObject(PhysicsObject *obj){
    _world->addRigidBody(obj->getPhysicsBody());
}

void PhysicsWorld::removePhysicsObject(PhysicsObject *obj){
    _world->removeRigidBody(obj->getPhysicsBody());
}

void PhysicsWorld::addRigidBody(btRigidBody* body){
    _world->addRigidBody(body);
}

void PhysicsWorld::removeRigidBody(btRigidBody* body){
    _world->removeRigidBody(body);
}

