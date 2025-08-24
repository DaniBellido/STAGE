#pragma once
#include <SDL.h>
#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
private:
	// Global private variables (Attributes) to use in different member functions
	bool isRunning;
	int millisecsPreviousFrame = 0;
	SDL_Window* window;
	SDL_Renderer* renderer;

	std::unique_ptr<Registry> registry;
	std::unique_ptr<AssetStore> assetStore;

public:
	// Constructor/Destructor
	Game();
	~Game();

	// Member Functions
	void Initialize();
	void Run();
	void LoadLevel(int level);
	void Setup();
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();

	int windowWidth;
	int windwHeight;

};

