#include "PhysicsObject.h"


// old constructor
//PhysicsObject::PhysicsObject(btCollisionShape* collider, vec3 objectPos, btScalar objMass)
//{
//    btCollisionShape* Collider = collider;
//    btDefaultMotionState* objectMotion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), Maths::glmToBullet(objectPos)));
//    btScalar objectMass = objMass;
//    btVector3 objectFallInertia = btVector3(0, 0, 0);
//    Collider->calculateLocalInertia(objectMass, objectFallInertia);
//
//    btRigidBody::btRigidBodyConstructionInfo objectRigidBodyCollisionInformation(objectMass, objectMotion, Collider, objectFallInertia);
//    _body = new btRigidBody(objectRigidBodyCollisionInformation);
//}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::applyForce(btVector3 direction){
    _body->applyCentralImpulse(direction);
}
void PhysicsObject::setPosition(btVector3 position){
    _position = _body->getWorldTransform().getOrigin();
    
    _body->translate(position - _position);
    
    _body->activate();
}
void PhysicsObject::translate(btVector3 position){
    _body->translate(position);
    _body->activate();
}
void PhysicsObject::setVelocity(btVector3 velocity){
    _body->setLinearVelocity(velocity);
}
btVector3 PhysicsObject::getVelocity(){
    return _body->getLinearVelocity();
}
btTransform PhysicsObject::getWorldTransForm(){
    return _body->getWorldTransform();
}

void PhysicsObject::toggleGravity(bool flag){
    if (flag == true){
        _body->setGravity(btVector3(0,0,-1));
    } else {
        _body->setGravity(btVector3());
    }
}

void PhysicsObject::Update(float dt)
{
	btTransform trans;
	_body->getMotionState()->getWorldTransform(trans);
	entity->position = Maths::bulletToGlm(btVector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
	
}


//! Construtor de PhysicsObject
/*!
 \param mass a massa do Objeto
 \param shape tipo do colisor PhysicsShape::
 \param position posição inicial.
 \param rotation rotação inicial
 \param scala escala do objeto.
 \param inecria vector 3 para definir estado inicial da fisica.
 \param mesh opicional referencia para a mesh que vai acompanhar o corpo fisico;
 \return PhysicsObject construido de acordo com os parametros fornecidos;
 */
PhysicsObject::PhysicsObject(float mass, PhysicsShape shape, btVector3 position, btVector3 rotation,
              btVector3 scale, btVector3 inercia, Entity* mesh){
    entity = mesh;
    _mass = mass;
    _position = position;
    _rotation = btQuaternion();
    _rotation.setEulerZYX(rotation.getX(), rotation.getY(), rotation.getZ());
    
    switch (shape){
        case PhysicsShape::Box:
            _shape = new btBoxShape(scale);
            break;
        case PhysicsShape::Capsule:
            _shape = new btCapsuleShape(scale.x(), scale.y());
            break;
        case PhysicsShape::Cyllinder:
            _shape = new btCylinderShape(scale);
            break;
        case PhysicsShape::Sphere:
            _shape = new btSphereShape(scale.x());
            break;
    }
    
    _motionState = new btDefaultMotionState(btTransform(this->_rotation, this->_position));
    this->_shape->calculateLocalInertia(this->_mass, this->_inercia);
    btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(this->_mass, this->_motionState, this->_shape, this->_inercia);
//    bodyCI.m_restitution = 0;
//    bodyCI.m_friction = 0.0;
    _body = new btRigidBody(bodyCI);
    
//    _body->setUserPointer((__bridge void*)self); //wtf is this
    
//    _body->setLinearFactor(btVector3(1,1,0 ));
//    _body->setAngularFactor(btVector3(0,0,0));
//    _body->setDamping(0.5, 0.5);
    
}

void PhysicsObject::Render() {

	MasterRenderer::GetInstance().processEntity(entity);

};