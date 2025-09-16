#pragma once
#include "../ECS/ECS.h"
#include "../Components/ProjectileComponent.h"

class ProjectileLifecicleSystem : public System
{
public:

	ProjectileLifecicleSystem() 
	{
		RequireComponent<ProjectileComponent>();
	}

	void Update() 
	{
		for (auto entity : GetSystemEntities()) 
		{
			auto projectile = entity.GetComponent<ProjectileComponent>();

			// Kill projectiles after they reach their duration limit
			if (SDL_GetTicks() - projectile.startTime > projectile.duration)
			{
				entity.Kill();
			}

		}
	}

};
