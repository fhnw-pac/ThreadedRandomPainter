#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <chrono>
#include <vector>

constexpr int sizeOfFpsArr = 5;

struct imageBuffer {
    uint8_t* buffer;
    size_t size;
};

class SDLWindow
{
    // SDL
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    // SDL TTF
    TTF_Font* font;
    SDL_Color fontColor = { 255, 0, 0, 255 };
    
    // FPS Counter
    SDL_Surface* fpsSurface;
    SDL_Texture* fpsTexture;
    SDL_Rect fpsPosition;
    std::chrono::high_resolution_clock::time_point tp;
    uint64_t fpsArr[5];
    unsigned fpsPtr;

    // infos
    std::string windowName;
    int imgW;
    int imgH;

    // data buff
    uint8_t* imageBuff;
    size_t buffSize;

public: 

    SDLWindow(int w, int h, std::string name);
    ~SDLWindow();

    void renderFrame();
    imageBuffer getImageBuffer();

};
