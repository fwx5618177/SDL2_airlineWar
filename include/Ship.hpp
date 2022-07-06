#ifndef Ship_hpp
#define Ship_hpp

#include "Actor.hpp"
#include "SpriteComponent.hpp"
#include "InputComponent.hpp"


class Ship : public Actor
{
public:
  Ship(class Game* game);
  ~Ship();
  void UpdateActor(float deltaTime) override;
  // void ActorInput(const uint8_t* keyState) override;
  void ProcessKeyboard(const uint8_t* state);
  float GetRightSpeed() const { return mRightSpeed; }
  float GetDownSpeed() const { return mDownSpeed; }
private:
  float mRightSpeed;
  float mDownSpeed;
  float mLaserCooldown;
  SpriteComponent* sc;
  InputComponent* ic;
};

#endif /* Ship_hpp */
