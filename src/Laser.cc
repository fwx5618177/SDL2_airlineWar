#include "Laser.hpp"

Laser::Laser(Game* game)
    :Actor(game)
    ,mDeathTimer(1.0f)
    ,sc(new SpriteComponent(this))
    ,mc(new MoveComponent(this))
    ,mCircle(new CircleComponent(this))
{
    sc->SetTexture(game->GetTexture("../Content/Textures/PlayerBullet.png"));

    mc->SetForwardSpeed(800.0f);

    mCircle->SetRadius(11.0f);
}

void Laser::UpdateActor(float deltaTime)
{
    // 超时的话，激光就已经衰退了
    mDeathTimer -= deltaTime;
    if (mDeathTimer <= 0.0f)
    {
        SetState(EDead);
    }
    else
    {
        for (auto ast : GetGame()->GetAsteroids())
        {
            if (Intersect(*mCircle, *(ast->GetCircle())))
            {
                // 消灭小行星
                SetState(EDead);
                ast->SetState(EDead);
                break;
            }
        }
    }
}

