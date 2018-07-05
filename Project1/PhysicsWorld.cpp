//
//  PhysicsWorld.cpp
//  OpenGLopinho2
//
//  Created by Thiago Dantas on 04/07/18.
//  Copyright © 2018 Thiago Dantas. All rights reserved.
//

#include "PhysicsWorld.h"

#include <stdio.h>

PhysicsWorld::PhysicsWorld(){
    _broadphase = new btDbvtBroadphase();
    _collisionConfiguration = new btDefaultCollisionConfiguration();
    _dispatcher = new btCollisionDispatcher(_collisionConfiguration);
    _solver = new btSequentialImpulseConstraintSolver();
    _world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfiguration);
    _world->setGravity(btVector3(0, -9.8, 0));
    _testDidCollided = false;
    _debugDrawer = new BulletDebugDrawer();
    _debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    _world->setDebugDrawer(_debugDrawer);
    
}

PhysicsWorld::~PhysicsWorld(){
    delete _world;
    delete _solver;
    delete _collisionConfiguration;
    delete _dispatcher;
    delete _broadphase;
    delete _debugDrawer;
}


//passar dt?
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
            printf("colidiu alguma coisa\n");
            // recuperando o objeto com o userpointer
            //antes de fazer isso todos os corpos adicionados tem que ser PhysicsObject;
//            PhysicsObject *physicsBodyA = (PhysicsObject*)bodyA->getUserPointer();
//            PhysicsObject *physicsBodyB = (PhysicsObject*)bodyB->getUserPointer();

//            if(physicsBodyA->tag == UnitTag::PlayerTag)
//            {
//
//                if(physicsBodyB->tag == UnitTag::EnemyTag)
//                {
//                    break;
//                }
//            }
            
        }
        
    }
    
}

void PhysicsWorld::debugDraw(){
    _world->debugDrawWorld();
}

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

