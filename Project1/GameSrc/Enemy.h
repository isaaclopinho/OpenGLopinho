#include "../PhysicsObject.h"
#include "../Timer.h"

class Enemy: public PhysicsObject{
    
public:
    Enemy(float mass, PhysicsShape shape, btVector3 inercia, Entity* e);
    ~Enemy();
    void RecieveDamage(int damage);
    bool PlayerNearby();
    Timer invulneravel;
    virtual void Update(float dt);
private:
    int health;
};
