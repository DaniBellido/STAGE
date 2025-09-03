#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <sol/sol.hpp>
#include <iostream>
#include <fstream>
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems/DamageSystem.h"


Game::Game() 
{
	isRunning = false;
	isDebug = false;
	registry = std::make_unique<Registry>();
	assetStore = std::make_unique<AssetStore>();
	eventBus = std::make_unique<EventBus>();
	Logger::Log("Game Constructor called!");
}

Game::~Game() 
{
	Logger::Log("Game Destructor called!");
}

void Game::Initialize() 
{
	// Initialize SDL libraries
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
	{
		// If init not possible output an error and return
		Logger::Err("Error initializing SDL.");
		return;
	}

	
	// SDL struct to store display mode information
	SDL_DisplayMode displayMode;

	// SDL function gets the current display mode of the first monitor and stores it in the displayMode structure
	SDL_GetCurrentDisplayMode(0, &displayMode);

	// Passing width and height values to class attributes
	windowWidth = 1280;               //displayMode.w;
	windwHeight = 720;               //displayMode.h;



	// Creating a Window (SDL Struct pointer)
	window = SDL_CreateWindow(
		"STAGE: Simple Two-dimensional Animation Game Engine",    // Window Title
		SDL_WINDOWPOS_CENTERED,					                  // Window X Position
		SDL_WINDOWPOS_CENTERED,					                  // Window Y Position
		windowWidth,									          // Window Width	
		windwHeight,									          // Window Height
		SDL_WINDOW_MAXIMIZED);					                  // Window Type          _BORDERLESS

	if (!window) 
	{
		// If window not created output an error and return
		Logger::Err("Error creating the SDL window.");
		return;
	}

	// Create a renderer (SDL Struct pointer) on the created window (frame)
	// second argument:  index of the rendering driver to use. -1 uses the default one
	// third argument: allows you to specify additional options or flags to use to accelerated GPU and VSync to
	// synchronize the frame rate of a game with a gaming monitor's refresh rate
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!renderer) 
	{
		// If renderer not created output an error and return
		Logger::Err("Error creating the SDL renderer.");
		return;
	}

	//Sets real full screen resolution
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	
	// Only true if creating window and renderer was successful 
	isRunning = true;
	
}

void Game::Run() 
{
	Setup();

	// Game Loop
	while (isRunning) 
	{
		ProcessInput();
		Update();
		Render();
		
	}

}

void Game::ProcessInput() 
{
	// Struct Event
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent))   // Passed as a reference to provide the memory address and not the whole struct
	{
		switch (sdlEvent.type) 
		{
			case SDL_QUIT:
				// Event to quit the application by closing the window [X]
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				// Event to quit the application by pressing ESC key
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) 
				{
					// Trigger that stops the Run() method breaking the Game Loop and proceeds to call Destroy() in main.cpp
					isRunning = false;
				}
				if (sdlEvent.key.keysym.sym == SDLK_d) 
				{
					isDebug = !isDebug;
				}
				break;

		
		}
	}
}

void Game::LoadLevel(int level) 
{
	// Add the systems that need to be processed in our game
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();
	registry->AddSystem<AnimationSystem>();
	registry->AddSystem<CollisionSystem>();
	registry->AddSystem<RenderColliderSystem>();
	registry->AddSystem<DamageSystem>();

	// Adding assets to the asset store
	assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
	assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
	assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
	assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper.png");
	assetStore->AddTexture(renderer, "radar-image", "./assets/images/radar.png");

	// Load the tilemap
	int tileSize = 32;
	double tileScale = 2.0;
	int mapNumCols = 25;
	int mapNumRows = 20;

	std::fstream mapFile;
	mapFile.open("./assets/tilemaps/jungle.map");

	for (int y = 0; y < mapNumRows; y++) 
	{
		for (int x = 0; x < mapNumCols; x++) 
		{
			char ch;
			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * tileSize;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;
			mapFile.ignore();

			Entity tile = registry->CreateEntity();
			tile.AddComponent<TransformComponent>
				(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
			tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, srcRectX, srcRectY);
		}
	}
	mapFile.close();


	// Create an entities and add components to the entity
	Entity chopper = registry->CreateEntity();
	chopper.AddComponent<TransformComponent>(glm::vec2(100.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
	chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1);
	chopper.AddComponent<AnimationComponent>(2, 10, true);

	Entity radar = registry->CreateEntity();
	radar.AddComponent<TransformComponent>(glm::vec2(windowWidth - 74, 10.0), glm::vec2(1.0, 1.0), 0.0);
	radar.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 2);
	radar.AddComponent<AnimationComponent>(8, 5, true);

	Entity tank = registry->CreateEntity();
	tank.AddComponent<TransformComponent>(glm::vec2(500.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(-30.0, 10.0));
	tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 1);
	tank.AddComponent<BoxColliderComponent>(32, 32);

	Entity truck = registry->CreateEntity();
	truck.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	truck.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 10.0));
	truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 2);
	truck.AddComponent<BoxColliderComponent>(32, 32);

	


}

void Game::Setup() 
{
	LoadLevel(1);

}

void Game::Update() 
{

	// If it's too fast, waste some clock cicles until we reach the MILLISECS_PER_FRAME constant
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);

	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) 
	{
		SDL_Delay(timeToWait);  // <---------------  Comment SDL_Delay to uncap FPS (Currently capped to 60FPS)
		
	}
	
	// The diference in ticks since the last frame, converted to seconds
	double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

	// Store the current frame time
	millisecsPreviousFrame = SDL_GetTicks();

	// Reset all event handlers for the current frame
	eventBus->Reset();

	// Perform the subscriptions of the events for all systems
	registry->GetSystem<DamageSystem>().SubscribeToEvent(eventBus);

	// Update the registry to process the entities that are waiting to be created/deleted
	registry->Update();

	// Invoke all the systems that need to update
	registry->GetSystem<MovementSystem>().Update(deltaTime);
	registry->GetSystem<AnimationSystem>().Update();
	registry->GetSystem<CollisionSystem>().Update(eventBus);

}

void Game::Render() 
{
	// Select an RGBa color
	SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
	// Clear renderer with the selected color
	SDL_RenderClear(renderer);

	// Invoke all the systems that need to render
	registry->GetSystem<RenderSystem>().Update(renderer, assetStore);
	if (isDebug) 
	{
		registry->GetSystem<RenderColliderSystem>().Update(renderer);
	}
	// Double-Buffered Render: Draw and display on screen all objects previously called swapping buffers in each frame
	SDL_RenderPresent(renderer); 
}

void Game::Destroy() 
{
	// Destroying renderer and window in the inverse order they were created
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}