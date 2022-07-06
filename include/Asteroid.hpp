
#ifndef Asteroid_hpp
#define Asteroid_hpp

#include "Actor.hpp"
#include "SpriteComponent.hpp"
#include "MoveComponent.hpp"
#include "CircleComponent.hpp"

class Asteroid : public Actor
{
public:
    Asteroid(class Game* game);
    ~Asteroid();
    
    class CircleComponent* GetCircle() { return mCircle; }
private:
    class CircleComponent* mCircle;
    class SpriteComponent* sc;
    class MoveComponent* mc;
};

#endif /* Asteroid_hpp */