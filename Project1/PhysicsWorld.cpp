//
//  PhysicsWorld.cpp
//  OpenGLopinho2
//
//  Created by Thiago Dantas on 04/07/18.
//  Copyright © 2018 Thiago Dantas. All rights reserved.
//

#include "PhysicsWorld.h"
#include "GameSrc/Enemy.h"
#include "GameSrc/Player.h"
#include "GameSrc/Hitbox.h"
#include "GameSrc/Boss.h"
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
            
            //se player for a  ou b;
            if(physicsBodyA->Is("Player"))
            {
                if (physicsBodyB->Is("Floor")){
//                    printf("colisão com chão");
                    Player::getInstance()->land();
                }
                if (physicsBodyB->Is("Enemy")){
                    Enemy *e = (Enemy*)bodyB->getUserPointer(); //pegar dano com inimigo?
                    Player::getInstance()->LoseHP(10, e->getWorldPosition());
                }
                //se tiver em cntato com ou colisor de inimigo e owner ativo;
                if (physicsBodyB->Is("EnemyTrigger")){
                    Hitbox* h = (Hitbox*) physicsBodyB;
                    Enemy *e = (Enemy*)h->owner; //pegar dano com inimigo?
                    if (e->entity->currentAnimation == "AttackDuplo"){
                        Player::getInstance()->LoseHP(e->damage, e->getWorldPosition());
                    }
                }
                if (physicsBodyB->Is("Boss")){
                    Boss *e = (Boss*)bodyB->getUserPointer(); //pegar dano com inimigo?
                    Player::getInstance()->LoseHP(10, e->getWorldPosition());
                }
            }else if(physicsBodyB->Is("Player"))
            {
                if (physicsBodyA->Is("Floor")){
                    Player::getInstance()->land();
                }
                if (physicsBodyA->Is("Enemy")){
                    Enemy *e = (Enemy*)bodyA->getUserPointer(); //pegar dano com inimigo?
                    Player::getInstance()->LoseHP(10, e->getWorldPosition());
                }
                //se tiver em cntato com ou colisor de inimigo e owner ativo;
                if (physicsBodyA->Is("EnemyTrigger")){
                    Hitbox* h = (Hitbox*) physicsBodyA;
                    Enemy *e = (Enemy*)h->owner; //pegar dano com inimigo?
                    if (e->entity->currentAnimation == "AttackDuplo"){
                        Player::getInstance()->LoseHP(e->damage, e->getWorldPosition());
                    }
                }
                if (physicsBodyA->Is("Boss")){
                    Boss *e = (Boss*)bodyA->getUserPointer(); //pegar dano com inimigo?
                    Player::getInstance()->LoseHP(10, e->getWorldPosition());
                }
            }
            
            // se A ou B for player trigger
            if(physicsBodyA->Is("Trigger")){
                if (physicsBodyB->Is("Projectile")){
                    
                    if (Player::getInstance()->entity.currentAnimation == "AtackDouble"){
                        physicsBodyB->setVelocity(physicsBodyB->getVelocity()*(-1));
                        physicsBodyB->type = "Reflect";
                    }
                }
                if (InputManager::GetInstance().ControllerButtonPress(X360_X) || (InputManager::GetInstance().KeyPress(SDLK_k))){
                    if (physicsBodyB->Is("Enemy")){
						Player::getInstance()->atacou = true;
                        Enemy *e = (Enemy*)bodyB->getUserPointer();
                        e->RecieveDamage(10);
                    }
                    if (physicsBodyB->Is("Boss")){
                        Boss *e = (Boss*)bodyB->getUserPointer();
                        e->RecieveDamage(200);
                    }
                    
                }
            } else if(physicsBodyB->Is("Trigger")){
                if (physicsBodyA->Is("Projectile")){
                   
                    if (Player::getInstance()->entity.currentAnimation == "AtackDouble"){
                        physicsBodyA->setVelocity(physicsBodyA->getVelocity()*(-1));
                        physicsBodyA->type = "Reflect";
                    }
                }
                if (InputManager::GetInstance().ControllerButtonPress(X360_X) || (InputManager::GetInstance().KeyPress(SDLK_k))){
                    if (physicsBodyA->Is("Enemy")){
                        Enemy *e = (Enemy*)bodyB->getUserPointer();
                        e->RecieveDamage(10);
                    }
                    if (physicsBodyA->Is("Boss")){
                        Boss *e = (Boss*)bodyA->getUserPointer();
                        e->RecieveDamage(200);
                    }
                    
                }
            }
            if (physicsBodyA->Is("Projectile")){
                if (physicsBodyB->Is("Player")){
                    Player::getInstance()->LoseHP(10, Maths::glmToBullet(Player::getInstance()->getForwardVector()));
                }
                if (!physicsBodyB->Is("Trigger")) physicsBodyA->RequestRemove();
                
                Player::getInstance()->LoseHP(10, Maths::glmToBullet(Player::getInstance()->getForwardVector()));
            } else if (physicsBodyB->Is("Projectile")){
                if (physicsBodyA->Is("Player")){
                    Player::getInstance()->LoseHP(10, Maths::glmToBullet(Player::getInstance()->getForwardVector()));
                }
                if (!physicsBodyA->Is("Trigger")) physicsBodyB->RequestRemove();
            }
            
            if (physicsBodyA->Is("Reflect")){
               // printf("projetil refletido colidiu");
                if (physicsBodyB->Is("Boss")){
                    Boss* b = (Boss *)physicsBodyB;
                    b->RecieveDamage(250);
                    physicsBodyA->RequestRemove();
                }
            } else if (physicsBodyB->Is("Reflect")){
              //  printf("projetil refletido colidiu");
                if (physicsBodyA->Is("Boss")){
                    Boss* b = (Boss *)physicsBodyA;
                    b->RecieveDamage(250);
                    physicsBodyB->RequestRemove();
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

