#include "Game.h"
#include <iostream>

Game::Game() 
{
	std::cout << "Game Constructor called!" << std::endl;
}

Game::~Game() 
{
	std::cout << "Game Destructor called!" << std::endl;
}

void Game::Initialize() 
{

}

void Game::Run() 
{
	while (true) 
	{
		ProcessInput();
		Update();
		Render();
	}

}

void Game::ProcessInput() 
{

}

void Game::Update() 
{

}

void Game::Render() 
{

}

void Game::Destroy() 
{

}