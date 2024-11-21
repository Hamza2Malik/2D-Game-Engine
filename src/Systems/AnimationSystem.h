#pragma once
#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Logger/Logger.h"
#include <SDL2/SDL.h>

class AnimationSystem: public System{
    public:
        AnimationSystem(){
            //ToDo 
            // RequiredComponents
            RequireComponent<SpriteComponent>();
            RequireComponent<AnimationComponent>();
        }

        void Update(){
            // Loop all entity that system instrested in
            for(auto entity: GetSystemEntities()){
                auto& animation = entity.GetComponent<AnimationComponent>();
                auto& sprite = entity.GetComponent<SpriteComponent>();

                animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate/1000) % animation.numFrames;
                sprite.srcRect.x = animation.currentFrame * sprite.width;
                //Logger::Log("Entity id = " + std::to_string(entity.GetId())+ " position is now (" + std::to_string(transform.position.x)+", "+ std::to_string(transform.position.y)+")");
            }
        }
};