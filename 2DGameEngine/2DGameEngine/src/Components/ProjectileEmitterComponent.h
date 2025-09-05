#pragma once
#include <glm/glm.hpp>
#include "SDL.h"

struct ProjectileEmitterComponent 
{
	glm::vec2 projectileVelocity;
	int repeatFrecuency;
	int projectileDuration;
	int hitPercentDamage;
	bool isFriendly;
	int lastEmissionTime;

	ProjectileEmitterComponent(
		glm::vec2 projectileVelocity = glm::vec2(0), 
		int repeatFrecuency = 0,
		int projectileDuration = 10000,
		int hitPercentDamage = 10,
		bool isFriendly = false
	) 
	{
		this->projectileVelocity = projectileVelocity;
		this->repeatFrecuency = repeatFrecuency;
		this->projectileDuration = projectileDuration;
		this->hitPercentDamage = hitPercentDamage;
		this->isFriendly = isFriendly;
		this->lastEmissionTime = SDL_GetTicks();
	}

};