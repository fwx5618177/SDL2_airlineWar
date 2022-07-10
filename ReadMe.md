# Mac C++开发

# 搭建环境
- SDL:Simple DirectMedia Layer, 一个跨平台的第三方库负责处理创建窗体、2D 图形、音频输出、键盘输入等过程。
    - 安装:`brew install SDL2`
    - 安装完成后，将`*.h`移动到本地的`external`

# 开发流程和阅读资料
- [Xcode与C++之游戏开发：Pong游戏](https://blog.csdn.net/guyu2019/article/details/87551008)

# SDL2 学习指南 - 初级

## 1. 流程

1. 生成窗口
```c++
if(SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Can't init video: %s", SDL_GetError());
    
    return 1;
}

SDL_Window* win = SDL_CreateWindow();

SDL_Delay(3000);
SDL_DestroyWindow(win);
```

- SDL_Init(SDL_INIT_VIDEO): 窗口本身就是调用系统的视频处理器
- SDL_Log("xx:%s", SDL_GetError()): 本身相当于printf, SDL_GetError则是获取错误
- SDL_CreateWindow: 创建窗口
- SDL_Delay: 延迟关闭窗口
- SDL_DestroyWindow: 销毁窗口

```c++
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
```
- SDL_PollEvent: 启动后获取窗口的时间类型

```c++

    SDL_Surface* sur = SDL_GetWindowSurface(mWindow);
    SDL_Rect rec = { 0,0,10,20 };
    SDL_FillRect(sur, &rec, 0xffff);

    ((uint32_t*) (sur->pixels))[100] = 0xff0000ff;

    SDL_UpdateWindowSurface(mWindow);

```
- SDL_GetWindowSurface: 渲染流程中，获取的时候屏幕所有像素点的信息。用于在渲染过程中对数据进行替换和更改
- 此种渲染方法可以对像素进行修改

```c++
mTicksCount = SDL_GetTicks();

while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
    ;

float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
if (deltaTime > 0.05f)
{
    deltaTime = 0.05f;
}
mTicksCount = SDL_GetTicks();
```
- SDL_GetTicks: 获取程序运行到现在的时间
- 在16ms内渲染，超出16ms则等待

2. 图像
```c++

// 清除上一次渲染
SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
SDL_RenderClear(mRenderer);

// todo
SDL_Texture * tex = LoadTexture("../Content/Textures/StarLayer.png");
SDL_RenderCopy(mRenderer, tex, 0, 0);

SDL_RenderPresent(mRenderer);

SDL_Texture*  Game::LoadTexture(const char* fileName) {
    // 从文件中加载
    SDL_Surface* surf = IMG_Load(fileName);

    if (!surf) {
        SDL_Log("加载图像文件 %s 失败", fileName);

        return nullptr;
    }

    // surface -> texture
    SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, surf);

    // 释放加载的图片
    SDL_FreeSurface(surf)

    if(!tex) {
        SDL_Log("%s surface 转换到 texture 失败: %s", fileName, SDL_GetError());
        return nullptr;
    }

    return tex;
}
```
- SDL_CreateTextureFromSurface: 加载到纹理
- SDL_RenderCopy: 混合渲染
- 如果使用bmp则不需要sdl——image库



3. 绘制文字
- 安装文字库: `brew install sdl2-ttf`

```c++
#include "SDL_ttf.h"

// 字体
TTF_Font* font;

SDL_Surface* fontSurface;

// text
SDL_Color color = {
    255,
    255,
    255,
    255
};
SDL_Texture* text = LoadFont("../Content/Fonts/AGENCYB.TTF", "Moxixi", color);
SDL_Rect src = { 0, 0, fontSurface->w, fontSurface->h };
SDL_RenderCopy(mRenderer, text, NULL, &src);

SDL_Texture* Game::LoadFont(const char* fileName, const char *textstr, SDL_Color color) {
    font = TTF_OpenFont(fileName, FONT_SIZE);

    if(font == nullptr) {
        SDL_Log("不能引入字体: %s", SDL_GetError());

        return nullptr;
    }

    fontSurface = TTF_RenderText_Blended(font, textstr, color);

    if (!fontSurface) {
        SDL_Log("加载失败: %s", fileName, SDL_GetError());

        return nullptr;
    }

    SDL_Texture* text = SDL_CreateTextureFromSurface(mRenderer, fontSurface);

    if(!text) {
        SDL_Log("%s surface 转换到 texture 失败: %s", fileName, SDL_GetError());
        return nullptr;
    }

    return text;
}
```


- SDL_RenderCopyEx: 可以实现对图片的改变