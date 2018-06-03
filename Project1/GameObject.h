#pragma once

#include <iostream>
#include <string>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

using namespace std;

class GameObject {

protected:
	bool remove;
	
public:
	string type;
	GameObject() { type = "GameObject"; };
	~GameObject() {};
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual btRigidBody* getRB() { return nullptr; };
	bool Remove() { return remove; };
	bool Is(string str) { return str == type; };
};