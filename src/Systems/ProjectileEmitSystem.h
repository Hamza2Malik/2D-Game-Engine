#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/CameraFollowComponent.h"
#include <SDL2/SDL.h>

class ProjectileEmitSystem: public System
{
public:
    ProjectileEmitSystem(){
        RequireComponent<ProjectileEmitterComponent>();
        RequireComponent<TransformComponet>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus){
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitSystem::OnKeyPressed);
    }

    void OnKeyPressed(KeyPressedEvent& event){
        if(event.symbol == SDLK_SPACE){
            Logger::Log("Space Pressed");
            for(auto entity: GetSystemEntities()){
                if(entity.HasComponent<CameraFollowComponent>()){
                    const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
                    const auto transform = entity.GetComponent<TransformComponet>();
                    const auto rigidbody = entity.GetComponent<RigidBodyComponent>();


                    glm::vec2 projectilePosition = transform.position;
                    if(entity.HasComponent<SpriteComponent>()){
                        auto sprite = entity.GetComponent<SpriteComponent>();
                        projectilePosition.x += (transform.scale.x * sprite.width / 2);
                        projectilePosition.y += (transform.scale.y * sprite.height / 2);
                    }

                    glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
                    int directionX = 0;
                    int directionY = 0;
                    if(rigidbody.velocity.x > 0) directionX = +1;
                    if(rigidbody.velocity.x < 0) directionX = -1;
                    if(rigidbody.velocity.y > 0) directionY = +1;
                    if(rigidbody.velocity.y < 0) directionY = -1;
                    projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
                    projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;

                    // Create new Entity and add it to world

                    Entity projectile = entity.registry->CreateEntity();
                    projectile.AddComponent<TransformComponet>(projectilePosition,glm::vec2(1.0,1.0),0);
                    projectile.AddComponent<RigidBodyComponent>(projectileVelocity);
                    projectile.AddComponent<SpriteComponent>("bullet-image",4,4,4);
                    projectile.AddComponent<BoxColliderComponent>(4,4);
                    projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentageDamage,projectileEmitter.projectileDuration);
                }
            }
        }
    }


    void Update(std::unique_ptr<Registry>& registry){
        for(auto entity: GetSystemEntities()){
            auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
            const auto transform = entity.GetComponent<TransformComponet>();

            if(projectileEmitter.repeatFrequency ==0){
                continue;
            }

            if(SDL_GetTicks()- projectileEmitter.lastEmissionTime > static_cast<Uint32>(projectileEmitter.repeatFrequency)){
                
                glm::vec2 projectilePosition = transform.position;
                if(entity.HasComponent<SpriteComponent>()){
                    const auto sprite = entity.GetComponent<SpriteComponent>();
                    projectilePosition.x += (transform.scale.x * sprite.width/2);
                    projectilePosition.y += (transform.scale.y * sprite.height/2);
                }

                Entity projectile = registry->CreateEntity();
                projectile.AddComponent<TransformComponet>(projectilePosition, glm::vec2(1.0, 1.0),0.0);
                projectile.AddComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
                projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
                projectile.AddComponent<BoxColliderComponent>(4,4);
                projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentageDamage,projectileEmitter.projectileDuration);

                
                projectileEmitter.lastEmissionTime = SDL_GetTicks();
            }
        }
    }
};
