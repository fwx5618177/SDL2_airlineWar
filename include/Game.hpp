#ifndef Game_hpp
#define Game_hpp

#include <SDL.h>
#include <SDL_image.h>

const int kThickness = 15;

const float kPaddleH = 100.0f;

struct Vector2 {
    float x;
    float y;
};

class Game {
    public:
        Game();
        bool Initialize();
        void RunLoop();
        void Shutdown();
    private:
        void ProcessInput();
        void UpdateGame();
        void GenerateOuput();
        void BallBounce(float deltaTime);

        SDL_Window* mWindow; // 创建window
        bool mIsRunning; // 继续运行
        SDL_Renderer* mRenderer; // 渲染器

        // 球拍
        Vector2 mPaddlePos;
        Vector2 mBallPos;
        Vector2 mBallVel; // 球的速度
        
        Uint32 mTicksCount; // 记录运行时间

        int mPaddleDir; // 球拍方向

};

#endif