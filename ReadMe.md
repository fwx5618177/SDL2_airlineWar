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
