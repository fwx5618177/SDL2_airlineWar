#include "Game.hpp"

using namespace std;

Game::Game()
: mWindow(nullptr), 
mIsRunning(true), 
mRenderer(nullptr), 
mTicksCount(0),
mPaddleDir(0) {

}

bool Game::Initialize() {
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);

    mPaddlePos.x = 10.0f;
    mPaddlePos.y = 768.0f / 2.0f;
    mBallPos.x = 1024.0f / 2.0f;
    mBallPos.y = 768.0f / 2.0f;

    // mBallVel.x = -200.0f;
    // mBallVel.y = 235.0f;

    mBallVel = {-200.0f, 235.0f};

    if (sdlResult != 0) {
        SDL_Log("Can't initialize SDL: %s", SDL_GetError());

        return false;
    }

    mWindow = SDL_CreateWindow(
        "Game Environment",
        100,
        100,
        1024,
        768,
        0
    );

    if (!mWindow) {
        SDL_Log("Create Window error: %s", SDL_GetError());

        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!mRenderer) {
        SDL_Log("Crate renderer error: %s", SDL_GetError());

        return false;
    }

    // 引入图像
    if(IMG_Init(IMG_INIT_PNG) == 0) {
        SDL_Log("Can't initialize SDL_image: %s", SDL_GetError());
    }


    return true;
}

void Game::Shutdown() {
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::RunLoop() {
    while(mIsRunning) {
        Game::ProcessInput();
        Game::UpdateGame();
        Game::GenerateOuput();
    }
}

void Game::ProcessInput() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                mIsRunning = false;
                break;
            
            default:
                break;
        }
    }

    // 获取键盘状态
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if(state[SDL_SCANCODE_ESCAPE]) {
        mIsRunning = false;
    }

    // 通过 w/s 更新球拍位置
    mPaddleDir = 0;

    if(state[SDL_SCANCODE_W]) {
        mPaddleDir -= 1;
    }
    
    if (state[SDL_SCANCODE_S]) {
        mPaddleDir += 1;
    }
}

void Game::GenerateOuput() {
    SDL_SetRenderDrawColor(
        mRenderer,
        129,
        216,
        209,
        255
    );


    SDL_RenderClear(mRenderer);

    // 绘制墙
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);

    SDL_Rect wall {
        0, // 左上 x 坐标
        0, // 左上 y 坐标
        1024, // 宽度
        kThickness // 高度
    };

    SDL_RenderFillRect(mRenderer, &wall);

    // 绘制底部墙
    wall.y = 768 - kThickness;
    SDL_RenderFillRect(mRenderer, &wall);
    
    // 绘制右边的墙
    SDL_Rect rightWall = { 1024 - kThickness, 0, kThickness, 1024 };

    SDL_RenderFillRect(mRenderer, &rightWall);


    // 绘制球拍
    SDL_Rect paddle {
        static_cast<int>(mPaddlePos.x),
        static_cast<int>(mPaddlePos.y - kPaddleH / 2),
        kThickness,
        static_cast<int>(kPaddleH)
    };

    SDL_RenderFillRect(mRenderer, &paddle);

    // 绘制球
    SDL_Rect ball {
        static_cast<int>(mBallPos.x - kThickness / 2),
        static_cast<int>(mBallPos.y - kThickness / 2),
        kThickness,
        kThickness
    };

    SDL_RenderFillRect(mRenderer, &ball);
    
    

    // 交换前后缓冲区
    SDL_RenderPresent(mRenderer);
}


void Game::UpdateGame() {

    // wait time gap equal 16ms
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)) ;

    // time gap
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

    // 固定时间增量的最大值
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }


    // 根据方向更新球拍位置
    if(mPaddleDir != 0) {
        mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;

        // limit
        if (mPaddlePos.y < (kPaddleH / 2.0f + kThickness)) {
            mPaddlePos.y = kPaddleH / 2.0f + kThickness;
        } else if(mPaddlePos.y > (768.0 - kPaddleH / 2.0f - kThickness)) {
            mPaddlePos.y = 768.0f - kPaddleH / 2.0f - kThickness;
        }
    }

    Game::BallBounce(deltaTime);

    mTicksCount = SDL_GetTicks();

}

void Game::BallBounce(float deltaTime) {
    mBallPos.x += mBallVel.x * 1.0f * deltaTime;
    mBallPos.y += mBallVel.y * 1.0f * deltaTime;

      // 球是否和顶部墙相碰
  if (mBallPos.y <= kThickness && mBallVel.y < 0.0f)
  {
    mBallVel.y *= -1;
  }
  else if (mBallPos.y >= (768 - kThickness) && mBallVel.y > 0.0f)
  {
    // 球和底部墙相碰
    mBallVel.y *= -1;
  }

  // 是否和球拍相交
  float diff = mPaddlePos.y - mBallPos.y;
  // 取绝对值
  diff = (diff > 0.0f) ? diff : -diff;
  if (
      // y分量差距足够小
      diff <= kPaddleH / 2.0f &&
      // 球拍的x范围内
      mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
      // 球正向左运动
      mBallVel.x < 0.0f
    )
  {
    mBallVel.x *= -1.0f;
  }
  // 如果球出了窗口，结束游戏
  else if (mBallPos.x <= 0.0f)
  {
    mIsRunning = false;
  }
  // 如果球碰到右边的墙，则反弹
  else if(mBallPos.x >= (1024.0f - kThickness) && mBallVel.x > 0.0f)
  {
    mBallVel.x *= -1.0f;
  }

}