#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Systems/ProjectileLifecycleSystem.h"
#include <glm/glm.hpp> 
#include <iostream>
#include <fstream>

int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;



Game::Game(){
    isRunning = false;
    isDebug =false; 
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
    eventBus = std::make_unique<EventBus>();
    Logger::Log("Hello I amn created");
} 

Game::~Game(){
    Logger::Err("Hello I am destroyed");
}

void Game::Initialize(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cerr <<"Error Initializing SDL";
        return;
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = 800; //displayMode.w;
    windowHeight = 600; //displayMode.h;
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_BORDERLESS
    );

    if(!window){
        std::cerr<<"Error creating SDL window"<<std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        std::cerr<<"Error creating SDL renderer"<<std::endl;
        return;
    }

    camera.x = 0;
    camera.y = 0;
    camera.w = windowWidth;
    camera.h = windowHeight;

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    isRunning = true;
}

void Game::ProcessInput(){
    SDL_Event sdlEvent;
    while(SDL_PollEvent(&sdlEvent)){
        switch(sdlEvent.type){
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if(sdlEvent.key.keysym.sym == SDLK_ESCAPE){
                    isRunning = false;
                }
                if(sdlEvent.key.keysym.sym == SDLK_d){
                    isDebug = !isDebug;
                }
                eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
                break;  
        }
    }    
}

void Game::LoadLevel(int level){

    // Add System need to process in game
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<RenderColliderSystem>();
    registry->AddSystem<DamageSystem>();
    registry->AddSystem<KeyboardControlSystem>();
    registry->AddSystem<CameraMovementSystem>();
    registry->AddSystem<ProjectileEmitSystem>();
    registry->AddSystem<ProjectileLifecycleSystem>();
    

    // Adding Asset to the asset store
    assetStore->AddTexture(renderer,"tank-image", "./assets/images/tank-panther-right.png");
    assetStore->AddTexture(renderer,"truck-image", "./assets/images/truck-ford-right.png");
    assetStore->AddTexture(renderer,"chopper-image", "./assets/images/chopper-spritesheet.png");
    assetStore->AddTexture(renderer,"radar-image", "./assets/images/radar.png");
    assetStore->AddTexture(renderer,"bullet-image", "./assets/images/bullet.png");

    //Asset for tiles
    assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");

    int tileSize = 32;
    double tileScale = 1.5;
    int mapNumCols = 25;
    int mapNumRows = 20;
    std::fstream mapFile;
    mapFile.open("./assets/tilemaps/jungle.map");

    for(int y = 0; y < mapNumRows; y++){
        for(int x = 0; x<mapNumCols; x++){
            char ch;
            mapFile.get(ch);
            int srcRectY = std::atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            mapFile.ignore();

            Entity tile = registry->CreateEntity();
            tile.AddComponent<TransformComponet>(glm::vec2(x*(tileScale*tileSize), y*(tileScale*tileSize)), glm::vec2(tileScale, tileScale), 0.0);
            tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0,false,srcRectX, srcRectY);
        }
    }
    mapFile.close();
    
    mapWidth = mapNumCols * tileScale * tileSize;
    mapHeight = mapNumRows * tileScale * tileSize;


    //Create a Entity
    Entity tank = registry->CreateEntity();
    //registry->AddComponent<TransformComponet>(tank, glm::vec2(10.0, 30.0), glm::vec2(1.0,1.0), 0.0);
    //registry->AddComponent<RigidBodyComponent>(tank, glm::vec2(50.0, 0.0));
    tank.AddComponent<TransformComponet>(glm::vec2(500.0, 10.0), glm::vec2(1.0,1.0), 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image",32,32,1);
    tank.AddComponent<BoxColliderComponent>(32,32);
    tank.AddComponent<ProjectileEmitterComponent>(glm::vec2(100.0,0.0), 5000,3000,0, false);
    tank.AddComponent<HealthComponent>(100);
    
    //tank
    //tank.RemoveComponent<TransformComponet>();

    Entity truck = registry->CreateEntity();
    truck.AddComponent<TransformComponet>(glm::vec2(10.0, 10.0), glm::vec2(1.0,1.0), 0.0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    truck.AddComponent<SpriteComponent>("truck-image",32,32,2);
    truck.AddComponent<BoxColliderComponent>(32,32);
    truck.AddComponent<ProjectileEmitterComponent>(glm::vec2(0.0,100.0), 2000,5000,0, false);
    truck.AddComponent<HealthComponent>(100);

    Entity chopper = registry->CreateEntity();
    chopper.AddComponent<TransformComponet>(glm::vec2(100.0, 100.0), glm::vec2(1.0,1.0), 0.0);
    chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    chopper.AddComponent<SpriteComponent>("chopper-image",32,32,1);
    chopper.AddComponent<AnimationComponent>(2,15,true);
    chopper.AddComponent<ProjectileEmitterComponent>(glm::vec2(150.0,150.0), 0,1000,0, true);
    chopper.AddComponent<KeyboardControlledComponent>(glm::vec2(0,-80), glm::vec2(80,0), glm::vec2(0,80), glm::vec2(-80,0));
    chopper.AddComponent<CameraFollowComponent>();
    chopper.AddComponent<HealthComponent>(100);

    Entity radar = registry->CreateEntity();
    radar.AddComponent<TransformComponet>(glm::vec2(windowWidth-74, 10.0), glm::vec2(1.0,1.0), 0.0);
    radar.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    radar.AddComponent<SpriteComponent>("radar-image",64,64,2, true);
    radar.AddComponent<AnimationComponent>(8,5,true);
    


    //tank.Kill();

}

void Game::Setup(){
    LoadLevel(1);
}

void Game::Update(){
    // Not effiient way it will take all the resources and still doing nothing
    //while(!SDL_TICKS_PASSED(SDL_GetTicks(), millisecPreviousFrame + MILLI_SECS_PER_FRAME));
    // This is fast way and tell operating system to use this resources till we are waiting.

    int timeToWait = MILLI_SECS_PER_FRAME - (SDL_GetTicks() - millisecPreviousFrame);

    if(timeToWait > 0 && timeToWait < MILLI_SECS_PER_FRAME){
        SDL_Delay(timeToWait);
    } 


    double deltaTime = (SDL_GetTicks() - millisecPreviousFrame)/1000.0;
    millisecPreviousFrame = SDL_GetTicks();

    // playerPosition.x += playerVelocity.x * deltaTime;  
    // playerPosition.y += playerVelocity.y * deltaTime; 
    //TODO
    //MovementSystem.Update( )
    
    // Reset all event handler for the current frame
    eventBus->Reset();

    //perform the subscription of the event for all system
    registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<KeyboardControlSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(eventBus);
    

    registry->Update();    

    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update(eventBus);
    registry->GetSystem<CameraMovementSystem>().Update(camera);
    registry->GetSystem<ProjectileEmitSystem>().Update(registry);
    registry->GetSystem<ProjectileLifecycleSystem>().Update();
    

}


void Game::Render(){
    SDL_SetRenderDrawColor(renderer,21,21,21,255);
    SDL_RenderClear(renderer);

    registry->GetSystem<RenderSystem>().Update(renderer, assetStore,camera);

    if(isDebug){
        registry->GetSystem<RenderColliderSystem>().Update(renderer, camera);
    }


    // // Draw a PNG Texture
    // SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
    // SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    // SDL_FreeSurface(surface);
    // SDL_Rect dstRect= {
    //     static_cast<int>(playerPosition.x), 
    //     static_cast<int>(playerPosition.y), 
    //     32, 
    //     32};
    // SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    // SDL_DestroyTexture(texture);
    SDL_RenderPresent(renderer);

    
}

void Game::Run(){
    Setup();
    while(isRunning){
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Destroy(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    

      
}

 
