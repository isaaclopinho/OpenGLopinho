#pragma once

#include "GameObject.h"
#include "Entity.h"
#include <iostream>
using namespace std;

class PhysicsObject : public GameObject {

	
	
	public:
		Entity * entity;
		PhysicsObject() {};
		PhysicsObject(btCollisionShape* collider, vec3 objectPos, btScalar objectMass);
		~PhysicsObject();

		virtual void Update(float dt) = 0;
		virtual void Render() = 0;
		
		btRigidBody* getRB() override {return rigidBody;};

private:

	void CheckInput();
	void PlayerMove();

	btRigidBody* rigidBody;
};