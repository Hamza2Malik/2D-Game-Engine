#pragma once

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h"
#include <SDL2/SDL.h>
#include <memory>

const int FPS = 60;
const int MILLI_SECS_PER_FRAME = 1000/ FPS; //how many sec each frame can take since 1sec have 1000 miliSec so we divide 1000/FPS to get sec to last each Frame.

class Game{
    private:
        int millisecPreviousFrame=0; 
        bool isRunning;
        bool isDebug;
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Rect camera;

        std::unique_ptr<Registry> registry;
        std::unique_ptr<AssetStore> assetStore;
        std::unique_ptr<EventBus> eventBus;
    
     



    public:
        Game();
        ~Game();
        void Initialize();
        void Run();
        void Setup();
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();
        void LoadLevel(int level);

        static int windowWidth;
        static int windowHeight;
        static int mapWidth;
        static int mapHeight;

};