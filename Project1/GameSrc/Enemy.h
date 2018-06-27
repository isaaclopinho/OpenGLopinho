#pragma once
#include "../PhysicsObject.h"

class Enemy : public PhysicsObject {

public:

	void Update(float dt) override;

protected:

	int health;
	bool isAlive;
	void CheckHealth();
};