#pragma once
#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include <memory>

class DamageSystem: public System{
    public:
        DamageSystem(){
            RequireComponent<BoxColliderComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus){
            eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
        }

        void onCollision(CollisionEvent& event){
            Logger::Log("The Damege system recieved an event collsion between entities " + std::to_string(event.a.GetId())+" and "+std::to_string(event.b.GetId()));
            // event.a.Kill();
            // event.b.Kill();
        }

        void Update(){

        }
};