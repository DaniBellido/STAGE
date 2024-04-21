#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <sol/sol.hpp>
#include <iostream>

Game::Game() 
{
	isRunning = false;
	std::cout << "Game Constructor called!" << std::endl;
}

Game::~Game() 
{
	std::cout << "Game Destructor called!" << std::endl;
}

void Game::Initialize() 
{
	// Initialize SDL libraries
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
	{
		// If init not possible output an error and return
		std::cerr << "Error initializing SDL." << std::endl;
		return;
	}

	// Creating a Window (SDL Struct pointer)
	window = SDL_CreateWindow(
		"STAGE: Simple Two-dimensional Animation Game Engine",    // Window Title
		SDL_WINDOWPOS_CENTERED,					                  // Window X Position
		SDL_WINDOWPOS_CENTERED,					                  // Window Y Position
		800,									                  // Window Width	
		600,									                  // Window Height
		SDL_WINDOW_MAXIMIZED);					                  // Window Type

	if (!window) 
	{
		// If window not created output an error and return
		std::cerr << "Error creating the SDL window." << std::endl;
		return;
	}

	// Create a renderer (SDL Struct pointer) on the created window (frame)
	// second argument:  index of the rendering driver to use. -1 uses the default one
	// third argument: allows you to specify additional options or flags for creating the renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) 
	{
		// If renderer not created output an error and return
		std::cerr << "Error creating the SDL renderer." << std::endl;
		return;
	}
	
	// Only true if creating window and renderer was successful 
	isRunning = true;
	
}

void Game::Run() 
{
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

void Game::Update() 
{
	// TODO: Update Game Objects
}

void Game::Render() 
{
	// Select an RGBa color
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 5);
	// Clear renderer
	SDL_RenderClear(renderer);

	// TODO: Render all game objects here

	// Draw and display objects and colors
	SDL_RenderPresent(renderer);
}

void Game::Destroy() 
{
	// Destroying renderer and window in the inverse order they were created
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}