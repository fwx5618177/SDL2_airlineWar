#ifndef Game_hpp
#define Game_hpp

#include <SDL.h>

const int kThickness = 15;

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
};

#endif