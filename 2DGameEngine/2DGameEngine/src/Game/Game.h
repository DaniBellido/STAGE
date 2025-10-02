#pragma once
#include <SDL.h>
#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h"

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
private:
	// Global private variables (Attributes) to use in different member functions
	bool isRunning;
	bool isDebug;
	int millisecsPreviousFrame = 0;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect camera;

	std::unique_ptr<Registry> registry;
	std::unique_ptr<AssetStore> assetStore;
	std::unique_ptr<EventBus> eventBus;

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

	static int windowWidth;
	static int windwHeight;
	static int mapWidth;
	static int mapHeight;

};

