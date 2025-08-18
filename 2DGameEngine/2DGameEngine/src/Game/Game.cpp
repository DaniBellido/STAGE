#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <sol/sol.hpp>
#include <iostream>
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"


Game::Game() 
{
	isRunning = false;
	registry = std::make_unique<Registry>();
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
	windowWidth = displayMode.w;
	windwHeight = displayMode.h;



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
				break;

		
		}
	}
}



void Game::Setup() 
{
	//TODO: 
	
	// Create an entity
	Entity tank = registry->CreateEntity();
	
	// Add components
	tank.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(50.0, 0.0));

	// Remove Component from the entity
	tank.RemoveComponent<TransformComponent>();
	


	//registry->AddComponent<struct TransformComponent>(tank, glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
	//registry->AddComponent<struct RigidBodyComponent>(tank, glm::vec2(50.0, 30.0));

	// Entity tank = regisry.CreateEntity();
	// tank.AddComponent<TransformComponent>();
	// tank.AddComponent<BoxColliderComponent>();
	// tank.AddComponent<SpriteComponent>("./assets/images/tank.png");
	
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


	// TODO: 
	// MovementSystem.Update();
	// CollisionSystem.Update();

}

void Game::Render() 
{
	// Select an RGBa color
	SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
	// Clear renderer with the selected color
	SDL_RenderClear(renderer);

	// #################### Render all game objects here ####################


	// TODO: Render game objects...



	// ######################################################################
	
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