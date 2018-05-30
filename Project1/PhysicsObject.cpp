#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(btCollisionShape* collider, vec3 objectPos, btScalar objMass)
{
	btCollisionShape* Collider = collider;
	btDefaultMotionState* objectMotion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), Maths::glmToBullet(objectPos)));
	btScalar objectMass = objMass;
	btVector3 objectFallInertia = btVector3(0, 0, 0);
	Collider->calculateLocalInertia(objectMass, objectFallInertia);

	btRigidBody::btRigidBodyConstructionInfo objectRigidBodyCollisionInformation(objectMass, objectMotion, Collider, objectFallInertia);
	rigidBody = new btRigidBody(objectRigidBodyCollisionInformation);

}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::Update(float dt)
{
	btTransform trans;
	rigidBody->getMotionState()->getWorldTransform(trans);
	entity->position = Maths::bulletToGlm(btVector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
	
}
