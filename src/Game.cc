#include "Game.hpp"
#include "SDL_image.h"
#include "Actor.hpp"
#include "BGSpriteComponent.hpp"
#include "Random.hpp"
#include "Asteroid.hpp"

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mIsRunning(true)
,mTicksCount(0)
{
    
}

bool Game::Initialize()
{
    // 初始化 SDL 库
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0)
    {
        SDL_Log("不能初始化 SDL: %s", SDL_GetError());
        return false;
    }
    
    // 创建 SDL 窗体
    mWindow = SDL_CreateWindow(
                               "Aircrafts",        // 标题
                               100,               // 窗体左上角的 x 坐标
                               100,               // 窗体左上角的 y 坐标
                               1024,              // 窗体宽度
                               768,               // 窗体高度
                               0                  // 标志位
                               );
    
    if (!mWindow)
    {
        SDL_Log("创建窗体失败: %s", SDL_GetError());
        return false;
    }
    
    // 创建渲染器
    mRenderer = SDL_CreateRenderer(
                                   mWindow,
                                   -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
                                   );
    if (!mRenderer)
    {
        SDL_Log("创建渲染器失败: %s", SDL_GetError());
        return false;
    }
    
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_Log("不能初始化SDL_image: %s", SDL_GetError());
        return false;
    }

    Random::Init();
    
    LoadData();
    mTicksCount = SDL_GetTicks();
    return true;
}

void Game::LoadData() {
    // 创建一个飞船player
     mShip = new Ship(this);
     mShip->SetPosition(Vector2(100.0f, 384.0f));
     mShip->SetScale(1.5f);

    // 创建敌人
    const int kNumAsteroids = 5;
    for (int i = 0; i < kNumAsteroids; i++)
    {
        new Asteroid(this);
    }
    
    // 为背景创建 actor (不需要子类)
    Actor* temp = new Actor(this);
    temp->SetPosition(Vector2(512.0f, 384.0f));
    
    // 创建一个遥远的深层背景
    BGSpriteComponent* bg = new BGSpriteComponent(temp);
    bg->SetScreenSize(Vector2(1024.0f, 768.0f));
    std::vector<SDL_Texture*> bgtexs = {
        GetTexture("../Content/Textures/StarLayer.png"),
        GetTexture("../Content/Textures/StarLayer.png")
    };
    bg->SetBGTextures(bgtexs);
    bg->SetScrollSpeed(-100.0f);
    
    // 创建一个更近的背景
    bg = new BGSpriteComponent(temp, 50);
    bg->SetScreenSize(Vector2(1024.0f, 768.0f));
    bgtexs = {
        GetTexture("../Content/Textures/Stars.png"),
        GetTexture("../Content/Textures/Stars.png")
    };
    bg->SetBGTextures(bgtexs);
    bg->SetScrollSpeed(-200.0f);
}

void Game::UnloadData()
{
    // 删除actor
    // 因为~Actor调用RemoveActor，所以必须使用循环
    while (!mActors.empty())
    {
        delete mActors.back();
    }
    
    // 销毁texture
    for (auto i : mTextures)
    {
        SDL_DestroyTexture(i.second);
    }
    mTextures.clear();
}

SDL_Texture* Game::LoadTexture(const char* fileName)
{
    // 从文件中加载
    SDL_Surface* surf = IMG_Load(fileName);
    
    if (!surf)
    {
        SDL_Log("加载图像文件 %s 失败", fileName);
        return nullptr;
    }
    
    // 从 surface 创建 texture
    SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, surf);
    SDL_FreeSurface(surf);
    if (!tex)
    {
        SDL_Log("%s surface 转换到 texture 失败!", fileName);
        return nullptr;
    }
    return tex;
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
    SDL_Texture* tex = nullptr;
    // texture是否已经存在？
    auto iter = mTextures.find(fileName);
    if (iter != mTextures.end())
    {
        tex = iter->second;
    }
    else
    {
        // 从文件中加载
        SDL_Surface* surf = IMG_Load(fileName.c_str());
        if (!surf)
        {
            SDL_Log("加载texture文件%s失败", fileName.c_str());
            return nullptr;
        }
        
        // 从 surface 中创建 textures
        tex = SDL_CreateTextureFromSurface(mRenderer, surf);
        SDL_FreeSurface(surf);
        if (!tex)
        {
            SDL_Log("无法把%s从surface转化到texture", fileName.c_str());
            return nullptr;
        }
        
        mTextures.emplace(fileName.c_str(), tex);
    }
    return tex;
}

void Game::Shutdown()
{
    UnloadData();
    IMG_Quit();
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput() {
    SDL_Event event;
    
    // 有 event 在队列就一直循环
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                mIsRunning = false;
                break;
            default:
                break;
        }
    }
    
    // 获取键盘的状态
    const Uint8* state = SDL_GetKeyboardState(NULL);
    // 如果按了 Esc，结束循环
    if (state[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }
    
    // 处理飞船的输入
    mShip->ProcessKeyboard(state);

    // mUpdatingActors = true;
    // for (auto actor : mActors)
    // {
    //     actor->ProcessInput(state);
    // }
    // mUpdatingActors = false;
}

void Game::UpdateGame()
{
    // 计算增量时间
    // 等待16ms
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;
    
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }
    mTicksCount = SDL_GetTicks();
    
    // 更新所有actor
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;
    
    // 移动待定actor到mActors
    for (auto pending : mPendingActors)
    {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();
    
    // 添加 dead actor 到临时向量
    std::vector<Actor*> deadActors;
    for (auto actor : mActors)
    {
        if (actor->GetState() == Actor::EDead)
        {
            deadActors.emplace_back(actor);
        }
    }
    
    // 删除处于dead的actor（从mActors中移除)
    for (auto actor : deadActors)
    {
        delete actor;
    }
    
}


void Game::GenerateOutput()
{
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);
    
    // 绘制所有精灵组件
    for (auto sprite : mSprites)
    {
        sprite->Draw(mRenderer);
        
    }
    
    SDL_RenderPresent(mRenderer);
    
}

void Game::AddActor(Actor* actor)
{
    // 如果正在更新actor，就添加到待处理列表里
    if (mUpdatingActors)
    {
        mPendingActors.emplace_back(actor);
    }
    else
    {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor* actor)
{
    // 是否在待定actor中
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end())
    {
        // 交换到尾部（避免复制)
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }
    
    // 是否在 actor中
    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
        // 交换到尾部（避免复制)
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

void Game::AddSprite(SpriteComponent* sprite)
{
    // 在有序向量中找到插入点
    // (第一个比传入的sprite的order顺序大的元素)
    int myDrawOrder = sprite->GetDrawOrder();
    auto iter = mSprites.begin();
    for ( ;
         iter != mSprites.end();
         ++iter)
    {
        if (myDrawOrder < (*iter)->GetDrawOrder())
        {
            break;
        }
    }
    
    // 在迭代器之前插入
    mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
    // (不能交换，不然顺序就没了)
    auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
    mSprites.erase(iter);
}

void Game::AddAsteroid(Asteroid* ast)
{
    mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
    auto iter = std::find(mAsteroids.begin(),
        mAsteroids.end(), ast);
    if (iter != mAsteroids.end())
    {
        mAsteroids.erase(iter);
    }
}