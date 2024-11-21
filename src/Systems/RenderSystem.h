#pragma once
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Logger/Logger.h"
#include "../AssetStore/AssetStore.h"
#include <SDL2/SDL.h>
#include <memory>
#include <algorithm>

class RenderSystem: public System{
    public:
        RenderSystem(){
            //ToDo 
            // RequiredComponents
            RequireComponent<TransformComponet>();
            RequireComponent<SpriteComponent>();
        }

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect& camera){
            // Sort all the entities of our system z-index
            struct RenderableEntity{
                TransformComponet transformComponet;
                SpriteComponent spriteComponent;
            };

            std::vector<RenderableEntity> renderableEntities;
            for(auto entity: GetSystemEntities()){
                RenderableEntity renderableEntity;
                renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
                renderableEntity.transformComponet = entity.GetComponent<TransformComponet>();
                renderableEntities.emplace_back(renderableEntity);
            }

            std::sort(renderableEntities.begin(), renderableEntities.end(), 
          [](const RenderableEntity& a, const RenderableEntity& b) {
              return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
          });



            //-------
            // Loop all entity that system instrested in
            for(auto entity: renderableEntities){
                const auto transform = entity.transformComponet;
                const auto sprite = entity.spriteComponent;
                
                SDL_Rect srcRect = sprite.srcRect;
                SDL_Rect dstRect = {
                    static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)),
                    static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)),
                    static_cast<int>(sprite.width * transform.scale.x),
                    static_cast<int>(sprite.height * transform.scale.y)
                };
                
                
                SDL_RenderCopyEx(
                    renderer,
                    assetStore->GetTexture(sprite.assetId),
                    &srcRect,
                    &dstRect,
                    transform.rotation,
                    NULL,
                    SDL_FLIP_NONE);
                // Draw Png Texture
            }
        }
};
