//
//  PhysicsWorld.h
//  OpenGLopinho2
//
//  Created by Thiago Dantas on 04/07/18.
//  Copyright © 2018 Thiago Dantas. All rights reserved.
//

#include <btBulletDynamicsCommon.h>
#include "PhysicsObject.h"
#include "BulletDebugDrawer.h"

#ifndef PhysicsWorld_h
#define PhysicsWorld_h

class PhysicsWorld {
    
    
public:
    PhysicsWorld();
    ~PhysicsWorld();
    
    void debugDraw();
    void updateWorld(float dt);
    void addPhysicsObject(PhysicsObject *obj);
    void addPhysicsObject(PhysicsObject *obj, PhysicsBitMask objBit, int collides = COL_WALL | COL_PLAYER | COL_ENEMY | COL_FLOOR);
    void removePhysicsObject(PhysicsObject *obj);
    void addRigidBody(btRigidBody *body);
    void removeRigidBody(btRigidBody *body);
    
private:
    BulletDebugDrawer*                      _debugDrawer;
    btBroadphaseInterface*                  _broadphase;
    btDefaultCollisionConfiguration*        _collisionConfiguration;
    btCollisionDispatcher*                  _dispatcher;
    btSequentialImpulseConstraintSolver*    _solver;
    btDiscreteDynamicsWorld*                _world;
    bool _testDidCollided;
};


#endif /* PhysicsWorld_h */
