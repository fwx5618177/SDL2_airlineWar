#ifndef Game_hpp
#define Game_hpp

#include <SDL.h>

const int kThickness = 15;

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

        SDL_Window* mWindow; // 创建window
        bool mIsRunning; // 继续运行
        SDL_Renderer* mRenderer; // 渲染器

        // 球拍
        Vector2 mPaddlePos;
        Vector2 mBallPos;
};

#endif