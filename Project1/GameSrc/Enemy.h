#include "../PhysicsObject.h"


class Enemy: public PhysicsObject{
    
public:
    Enemy(float mass, PhysicsShape shape, btVector3 inercia, Entity* e);
    ~Enemy();
    void RecieveDamage(int damage);
private:
    int health;
};
