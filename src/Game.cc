#include "Game.hpp"

using namespace std;

Game::Game(): mWindow(nullptr), mIsRunning(true), mRenderer(nullptr) {

}

bool Game::Initialize() {
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);

    mPaddlePos.x = 10.0f;
    mPaddlePos.y = 768.0f / 2.0f;
    mBallPo.x = 1024.0f / 2.0f;
    mBallPo.y = 768.0f / 2.0f;

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
        // Game::UpdateGame();
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
    wall = {
        1024 - kThickness,
        0,
        kThickness,
        1024
    };
    SDL_RenderFillRect(mRenderer, &wall);


    // 绘制球拍
    

    // 交换前后缓冲区
    SDL_RenderPresent(mRenderer);
}
