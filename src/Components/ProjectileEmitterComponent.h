#pragma once
#include<glm/glm.hpp>
#include<SDL2/SDL.h>

struct ProjectileEmitterComponent
{
    glm::vec2 projectileVelocity;
    int repeatFrequency;
    int projectileDuration;
    int hitPercentageDamage;
    bool isFriendly;
    int lastEmissionTime;
    ProjectileEmitterComponent(glm::vec2 projectileVelocity = glm::vec2(0),int repeatFrequency=0, int projectileDuration=10000,int hitPercentageDamage = 10,bool isFriendly=false){
        this->projectileVelocity = projectileVelocity;
        this->repeatFrequency = repeatFrequency;
        this->projectileDuration = projectileDuration;
        this->hitPercentageDamage = hitPercentageDamage;
        this->isFriendly = isFriendly;
        this->lastEmissionTime = SDL_GetTicks();
    }
};
