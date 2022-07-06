#include "Ship.hpp"
#include "AnimSpriteComponent.hpp"
#include "Game.hpp"
#include "Laser.hpp"

Ship::Ship(Game* game)
:Actor(game)
,mRightSpeed(0.0f)
,mDownSpeed(0.0f)
, mLaserCooldown(0.0f)
, sc(new SpriteComponent(this, 150))
, ic(new InputComponent(this))
{
  // 创建一个动画精灵组件
  AnimSpriteComponent* asc = new AnimSpriteComponent(this);
  std::vector<SDL_Texture*> anims = {
    game->GetTexture("../Content/Textures/Player_1.png"),
    // game->GetTexture("../Content/Textures/Player_1.png"),
    // game->GetTexture("../Content/Textures/Player_1.png"),
    // game->GetTexture("../Content/Textures/Player_2.png"),
    // game->GetTexture("../Content/Textures/Player_3.png"),
  };
  asc->SetAnimTextures(anims);

  // sc->SetTexture(game->GetTexture("../Content/Textures/Player_1.png"));

    // 设置按键
  ic->SetForwardKey(SDL_SCANCODE_W);
  ic->SetBackKey(SDL_SCANCODE_S);
  ic->SetClockwiseKey(SDL_SCANCODE_A);
  ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
  ic->SetMaxForwardSpeed(300.0f);
  ic->SetMaxAngularSpeed(Math::TwoPi);
}

Ship::~Ship()
{
    if (sc) delete sc;
    if (ic) delete ic;
}

void Ship::UpdateActor(float deltaTime)
{
  Actor::UpdateActor(deltaTime);
  // 基于速度和增量时间更新位置
  Vector2 pos = GetPosition();
  pos.x += mRightSpeed * deltaTime;
  pos.y += mDownSpeed * deltaTime;
  
  // 限制在屏幕左半部分
  if (pos.x < 25.0f)
  {
    pos.x = 25.0f;
  }
  else if (pos.x > 500.0f)
  {
    pos.x = 500.0f;
  }
  if (pos.y < 25.0f)
  {
    pos.y = 25.0f;
  }
  else if (pos.y > 743.0f)
  {
    pos.y = 743.0f;
  }
  SetPosition(pos);
}

void Ship::ProcessKeyboard(const uint8_t* state)
{
  mRightSpeed = 0.0f;
  mDownSpeed = 0.0f;
  // 右/左
  if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
  {
    mRightSpeed += 250.0f;
  }
  if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
  {
    mRightSpeed -= 250.0f;
  }
  // 上/下
  if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])
  {
    mDownSpeed += 300.0f;
  }
  if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])
  {
    mDownSpeed -= 300.0f;
  }
}

// void Ship::UpdateActor(float deltaTime)
// {
//     mLaserCooldown -= deltaTime;
// }


void Ship::ActorInput(const uint8_t* keyState)
{
    if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
    {
        // 创建激光
        Laser* laser = new Laser(GetGame());
        laser->SetPosition(GetPosition());
        laser->SetRotation(GetRotation());

        // 设定冷却期
        mLaserCooldown = 0.5f;
    }
}
