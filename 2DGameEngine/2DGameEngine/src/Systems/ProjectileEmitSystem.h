#pragma once

#include "../ECS/ECS.h"
#include "SDL.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"

class ProjectileEmitSystem : public System 
{
public:
	ProjectileEmitSystem() 
	{
		RequireComponent<ProjectileEmitterComponent>();
		RequireComponent<TransformComponent>();

	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) 
	{
		eventBus->SubscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitSystem::OnKeyPressed);
	}

	void OnKeyPressed(KeyPressedEvent& event) 
	{
		if (event.symbol == SDLK_SPACE) 
		{

			for (auto entity : GetSystemEntities()) 
			{
				// Only the entity with cameraFollowComponent
				if (entity.HasComponent<CameraFollowComponent>()) 
				{
					// Getting component values from the parent entity
					const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
					const auto transform = entity.GetComponent<TransformComponent>();
					const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

					// If parent entity has sprite, start the projectile position in the middle
					glm::vec2 projectilePosition = transform.position;
					if (entity.HasComponent<SpriteComponent>()) 
					{
						auto sprite = entity.GetComponent<SpriteComponent>();
						projectilePosition.x += (transform.scale.x * sprite.width / 2);
						projectilePosition.y += (transform.scale.y * sprite.height / 2);
					}

					// If parent entity direction is controled by the keyboard keys, modify direction of the projectile
					glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
					int directionX = 0;
					int directionY = 0;
					if (rigidbody.velocity.x > 0) directionX = +1;
					if (rigidbody.velocity.x < 0) directionX = -1;
					if (rigidbody.velocity.y > 0) directionY = +1;
					if (rigidbody.velocity.y < 0) directionY = -1;
					projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
					projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;

					// Create a new projectile entity and add it to the game
					Entity projectile = entity.registry->CreateEntity();
					projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
					projectile.AddComponent<RigidBodyComponent>(projectileVelocity);
					projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
					projectile.AddComponent<BoxColliderComponent>(4, 4);
					projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);

				}
			}
		}
	}

	void Update(std::unique_ptr<Registry>& registry) 
	{
		for (auto entity : GetSystemEntities()) 
		{
			auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
			const auto transform = entity.GetComponent<TransformComponent>();

			if (projectileEmitter.repeatFrecuency == 0) 
			{
				continue;
			}

			if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFrecuency) 
			{
				glm::vec2 projectilePosition = transform.position;

				if (entity.HasComponent<SpriteComponent>()) 
				{
					const auto sprite = entity.GetComponent<SpriteComponent>();
					projectilePosition.x += (transform.scale.x * sprite.width / 2);
					projectilePosition.y += (transform.scale.y * sprite.height / 2);
				}

				// Add a new projecile entity to the registry
				Entity projectile = registry->CreateEntity();
				projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
				projectile.AddComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
				projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
				projectile.AddComponent<BoxColliderComponent>(4, 4);
				projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);

				// Update the projectile emitter component last emission to the current miliseconds
				projectileEmitter.lastEmissionTime = SDL_GetTicks();
			}
		}
	}
};