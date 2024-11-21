#include "AssetStore.h"
#include "../Logger/Logger.h"
#include <SDL2/SDL_image.h>

AssetStore::AssetStore()
{
    Logger::Log("AssetStore constructor called!");
}

AssetStore::~AssetStore()
{
    ClearAssets();
    Logger::Log("AssetStore destructor called!");
}

void AssetStore::ClearAssets()
{
    // We don't do only texture.clear(); we have to free all the texture poiunter
    for(auto texture: textures){
        SDL_DestroyTexture(texture.second);
    }

    textures.clear();
}

void AssetStore::AddTexture(SDL_Renderer* renderer,const std::string &assetId, const std::string &filePath)
{
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    // Add txture to map
    textures.emplace(assetId, texture); 

    Logger::Log("New Texture Added to AssetStore  with id = "+ assetId);
}


SDL_Texture* AssetStore::GetTexture(const std::string &assetId)
{ 
    return textures[assetId];
}
