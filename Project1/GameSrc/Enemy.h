#include "../PhysicsObject.h"
#include "../Timer.h"

class Enemy: public PhysicsObject{
    
    enum EnemyStates {
        IDLE,
        WALKING,
        ATTACKING
    };
    
    EnemyStates currentState;
    
public:
    Enemy(float mass, PhysicsShape shape, btVector3 inercia, Entity* e);
    ~Enemy();
    void RecieveDamage(int damage);
    bool PlayerNearby();
    Timer invulneravel;
    Timer walkCD;
    Timer attCD;
    Timer atkAnimation;
    virtual void Update(float dt);
    int damage = 10;

	vec2 limitX = vec2();
	vec2 limitZ = vec2();

private:
    int health;
};
