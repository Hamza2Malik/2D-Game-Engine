#pragma once
#include<map>
#include<string>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>


class AssetStore{
    private:
    std::map<std::string, SDL_Texture*> textures;

    public:
    AssetStore();
    ~AssetStore();


    void ClearAssets();
    void AddTexture(SDL_Renderer* renderer,const std::string& assetId, const std::string& filePath);
    SDL_Texture* GetTexture(const std::string& assetId);

};
