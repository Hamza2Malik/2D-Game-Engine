#pragma once
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Logger/Logger.h"


class MovementSystem: public System{
    public:
        MovementSystem(){
            //ToDo 
            // RequiredComponents
            RequireComponent<TransformComponet>();
            RequireComponent<RigidBodyComponent>();
        }

        void Update(double deltaTime){
            // Loop all entity that system instrested in
            for(auto entity: GetSystemEntities()){
                auto& transform = entity.GetComponent<TransformComponet>();
                const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

                transform.position.x += rigidbody.velocity.x * deltaTime;
                transform.position.y += rigidbody.velocity.y * deltaTime;
                //Logger::Log("Entity id = " + std::to_string(entity.GetId())+ " position is now (" + std::to_string(transform.position.x)+", "+ std::to_string(transform.position.y)+")");
            }
        }
};