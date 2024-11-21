#pragma once 
#include <SDL2/SDL.h>

struct ProjectileComponent
{
    bool isFriendly;
    int hitPercentageDamage;
    int duration;
    int startTime;

    ProjectileComponent(bool isFriendly = false,int hitPercentageDamage = 0,int duration = 0){
        this->isFriendly = isFriendly;
        this->hitPercentageDamage = hitPercentageDamage;
        this->duration = duration;
        this->startTime = SDL_GetTicks();
    }
};
