#include "ECS.h"
#include "../Logger/Logger.h"
#include <algorithm>


int IComponent::nextId = 0;

int Entity::GetId() const{
    return id;
}


void Entity::Kill(){
    registry->KillEntity(*this);
}
void System::AddEntityToSytem(Entity entity){
    entities.push_back(entity);
}
void System::RemoveEntityfromSytem(Entity entity){
    entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other){
        return entity == other;
    }), entities.end()); 
}
std::vector<Entity> System::GetSystemEntities() const{
    return entities;
}
const Signature System::GetComponentSignature() const {
    return componentSignature;
}

Entity Registry::CreateEntity()
{
    int entityId;
    if(freeIds.empty()){
    entityId = numEntities++;
    if(entityId >= static_cast<int>(entityComponentSignatures.size())){
        entityComponentSignatures.resize(entityId+1);
    }  }
    else{
        entityId = freeIds.front();
        freeIds.pop_front();
    }
    Entity entity(entityId);
    entity.registry = this;
    entitiesToBeAdded.insert(entity);
    // make sure rhr entityComponentSignature vector can accomodate the new entity
    Logger::Log("Entity Created with id = " + std::to_string(entityId));
    return entity;
}

void Registry::KillEntity(Entity entity){
    entitiesToKilled.insert(entity);
     
}

void Registry::Update(){
    // ADD thr entities that are waiting to be created to the actiuve system
    for(auto entity: entitiesToBeAdded){
        AddEntityToSystem(entity);
    }
    entitiesToBeAdded.clear();
    // Processing the entities to killed

    for(auto entity : entitiesToKilled){
        RemoveEntityFromSystem(entity);

        entityComponentSignatures[entity.GetId()].reset();
        // MAke the entity id available to reuse.
        freeIds.push_back(entity.GetId());
    }

    entitiesToKilled.clear();
}   

void Registry::AddEntityToSystem(Entity entity){
    const auto entityId = entity.GetId();
    // TODO match entitiyComponrntsignature <-----> systemComponentSignature
    const auto& entityComponentSignature = entityComponentSignatures[entityId];

    // Loop all the system

    for(auto& system: systems){
        const auto& systemComponentSignature = system.second->GetComponentSignature();

        bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

        if(isInterested){
            system.second->AddEntityToSytem(entity);
        } 
    }
}


void Registry::RemoveEntityFromSystem(Entity entity){
    for(auto system: systems){
        system.second->RemoveEntityfromSytem(entity);
    }
}