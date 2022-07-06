#ifndef Laser_hpp
#define Laser_hpp

#include "Actor.hpp"
#include "Game.hpp"
#include "SpriteComponent.hpp"
#include "MoveComponent.hpp"
#include "CircleComponent.hpp"

class Laser : public Actor
{
public:
    Laser(class Game* game);

    void UpdateActor(float deltaTime) override;
private:
    float mDeathTimer;
    
    class CircleComponent* mCircle;
    class SpriteComponent* sc;
    class MoveComponent* mc;
};

#endif /* Laser_hpp */
