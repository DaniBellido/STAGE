#pragma once
#include <SDL.h>

class Game
{
private:
	// Global private variables (Attributes) to use in different member functions
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;

public:
	// Constructor/Destructor
	Game();
	~Game();

	// Member Functions
	void Initialize();
	void Run();
	void Setup();
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();

	int windowWidth;
	int windwHeight;

};

