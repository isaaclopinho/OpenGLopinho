#pragma once

#include "GameObject.h"

class PhysicsObject : public GameObject {



	
	protected:
		bool remove;
	public:
		PhysicsObject() {};
		~PhysicsObject() {};
		virtual void Update(float dt) = 0;
		virtual void Render() = 0;
		bool Remove() { return remove; };
};