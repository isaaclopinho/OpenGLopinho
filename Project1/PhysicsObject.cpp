#include "PhysicsObject.h"

//! Construtor de PhysicsObject
/*!
 \param mass a massa do Objeto
 \param shape tipo do colisor PhysicsShape::
 \param inecria (velocidade inicial).
 \param mesh de onde vamos recuperar o transform.
 \return PhysicsObject construido de acordo com os parametros fornecidos;
 */
PhysicsObject::PhysicsObject(float mass, PhysicsShape shape, btVector3 inercia, Entity* e): _mass(mass), _inercia(inercia), _position(Maths::glmToBullet(e->position)){
    entity = e;
    btVector3 rotation = Maths::glmToBullet(e->rotation);
    _rotation = btQuaternion();
    _rotation.setEulerZYX(rotation.getX(), rotation.getY(), rotation.getZ());
    btVector3 scale = Maths::glmToBullet(e->scale);
    switch (shape){
        case PhysicsShape::Box:
            //height, width, depth
            _shape = new btBoxShape(scale);
            break;
        case PhysicsShape::Capsule:
            // radius, height
            _shape = new btCapsuleShape(scale.x(), scale.y());
            break;
        case PhysicsShape::Cyllinder:
            //radius bottom, height, radius top
            _shape = new btCylinderShape(scale);
            break;
        case PhysicsShape::Sphere:
            //radius
            _shape = new btSphereShape(scale.x());
            break;
    }
    
    _motionState = new btDefaultMotionState(btTransform(this->_rotation, this->_position));
    this->_shape->calculateLocalInertia(this->_mass, this->_inercia);
    btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(this->_mass, this->_motionState, this->_shape, this->_inercia);
    //    bodyCI.m_restitution = 0;
    //    bodyCI.m_friction = 0.0;
    _body = new btRigidBody(bodyCI);
    
    _body->setUserPointer(this); //ponteiro para a classe que contém o rigidBody
    //    _body->setLinearFactor(btVector3(1,1,0 ));
    //    _body->setAngularFactor(btVector3(0,0,0));
    //    _body->setDamping(0.5, 0.5);
    type = "Floor";
}


//! Construtor de PhysicsObject
/*!
 \param mass a massa do Objeto
 \param shape tipo do colisor PhysicsShape::
 \param position posição inicial.
 \param rotation rotação inicial
 \param scala escala do objeto.
 \param inecria (velocidade inicial).
 \param mesh opicional referencia para a mesh que vai acompanhar o corpo fisico;
 \return PhysicsObject construido de acordo com os parametros fornecidos;
 */
PhysicsObject::PhysicsObject(float mass, PhysicsShape shape, btVector3 position, btVector3 rotation,
                             btVector3 scale, btVector3 inercia, Entity* e): _mass(mass), _position(position){
    entity = e;
    _rotation = btQuaternion();
    _rotation.setEulerZYX(rotation.getX(), rotation.getY(), rotation.getZ());
    
    switch (shape){
        case PhysicsShape::Box:
            //height, width, depth
            _shape = new btBoxShape(scale);
            break;
        case PhysicsShape::Capsule:
            _shape = new btCapsuleShape(scale.x(), scale.y());
            break;
        case PhysicsShape::Cyllinder:
            //radius, height
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
    bodyCI.m_friction = 0.8;
    _body = new btRigidBody(bodyCI);
    
    _body->setUserPointer(this); //ponteiro para a classe que contém o rigidBody
//    _body->setLinearFactor(btVector3(1,1,0 ));
//    _body->setAngularFactor(btVector3(0,0,0));
//    _body->setDamping(0.5, 0.5);
    type = "Floor";
}

PhysicsObject::~PhysicsObject()
{
    if (entity != NULL){
        delete entity;
    }
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

btVector3 PhysicsObject::getWorldPosition(){
    return _body->getWorldTransform().getOrigin();
}

btVector3 PhysicsObject::getWorldRotation(){
    btScalar rot[3];
    btMatrix3x3 rotMatrix = btMatrix3x3(_body->getWorldTransform().getRotation());
    rotMatrix.getEulerZYX(rot[0], rot[1], rot[2]);
    return btVector3(rot[0], rot[1], rot[2]);
}

void PhysicsObject::Update(float dt)
{
    if (entity != NULL) {
        entity->Update(dt);
//        entity->rotation = Maths::bulletToGlm(getWorldRotation()); //corpo fisico do player nao e alterado no update. nao funfa.
//        entity->rotation.x -= 90;
        entity->position = Maths::bulletToGlm(getWorldPosition());
    }
	
}

void PhysicsObject::Render() {

	MasterRenderer::GetInstance().processEntity(entity);

};


/// brief: true = Ativa/false = Desativa Contato (i.e: corpo atravessa ou não os outros, mas colide);
void PhysicsObject::toggleContact(bool flag){
    int flags = _body->getCollisionFlags();
    int contact = btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE;
    flags = (flag == false ? flags | contact : flags & (!contact));
    _body->setCollisionFlags(flags);
}

void PhysicsObject::SetRotation(btVector3 rotation){
    _rotation = btQuaternion();
    _rotation.setEulerZYX(rotation.getX(), rotation.getY(), rotation.getZ());
}
