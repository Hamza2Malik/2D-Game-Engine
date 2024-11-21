#pragma once 
#include<string>
#include<SDL2/SDL.h>

struct SpriteComponent{
    int width;
    int height;
    std::string assetId;
    SDL_Rect srcRect;
    int zIndex;
    bool isFixed;

    SpriteComponent(const std::string assetId="",int width=0,int height=0,int zIndex=0, bool isFixed = false ,int srcRectX =0, int srcRectY =0){
        this->width = width;
        this->height = height;
        this->assetId = assetId;
        this->srcRect = {srcRectX, srcRectY,width, height};
        this->zIndex = zIndex;
        this->isFixed = isFixed;
    }
};