#pragma once

#include <SDL.h>

struct ProjectileComponent
{
	bool isFriendly;
	int hitPercentGamage;
	int duration;
	int startTime;

	ProjectileComponent(bool isFriendly = false, int hitPercentDamage = 0, int duration = 0) 
	{
		this->isFriendly = isFriendly;
		this->hitPercentGamage = hitPercentDamage;
		this->duration = duration;
		this->startTime = SDL_GetTicks();
	}
};