#pragma once

#include "../ECS/ECS.h"
#include "../Components/ProjectileComponent.h"
#include <SDL2/SDL.h>

class ProjectileLifecycleSystem : public System{
    public:
        ProjectileLifecycleSystem(){
            RequireComponent<ProjectileComponent>();
        }

        void Update(){
            for(auto entity: GetSystemEntities()){
                auto projectile = entity.GetComponent<ProjectileComponent>();

                if(SDL_GetTicks() - projectile.startTime > static_cast<Uint32>(projectile.duration)){
                    entity.Kill();
                }
            }
        }
};