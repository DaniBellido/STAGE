#pragma once

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/HealthComponent.h"
#include"../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class DamageSystem : public System 
{
public: 

	DamageSystem() 
	{
		RequireComponent<BoxColliderComponent>();
	}

	void SubscribeToEvent(std::unique_ptr<EventBus>& eventBus) 
	{
		eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
	}

	void onCollision(CollisionEvent& event) 
	{
		Entity a = event.a;
		Entity b = event.b;

		Logger::Log("Collision event emitted: " + std::to_string(a.GetId()) + " and " + std::to_string(b.GetId()));

		if (a.BelongsToGroup("projectiles") && b.HasTag("player")) 
		{
			OnProjectileHitsPlayer(a, b); // "a" is the projectile and "b" is the player. 
		}

		if (b.BelongsToGroup("projectiles") && a.HasTag("player"))
		{
			OnProjectileHitsPlayer(b, a); // "b" is the projectile and "a" is the player. 
		}

		if (a.BelongsToGroup("projectiles") && b.BelongsToGroup("enemies"))
		{
			OnProjectileHitsEnemy(a, b);
		}

		if (b.BelongsToGroup("projectiles") && a.BelongsToGroup("enemies"))
		{
			OnProjectileHitsEnemy(b, a);
		}
	}

	void OnProjectileHitsPlayer(Entity projectile, Entity player) 
	{
		auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

		if (!projectileComponent.isFriendly) 
		{
			// Reduce the health of the player by the projectile hitPercentDamage
			auto& health = player.GetComponent<HealthComponent>();
			// Substract player's health
			health.healthPercentage -= projectileComponent.hitPercentDamage;

			if (health.healthPercentage <= 0) 
			{
				player.Kill();
			}

			projectile.Kill();
		}
	}

	void OnProjectileHitsEnemy(Entity projectile, Entity enemy) 
	{
		auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

		if (projectileComponent.isFriendly) 
		{
			auto& health = enemy.GetComponent<HealthComponent>();

			health.healthPercentage -= projectileComponent.hitPercentDamage;

			if (health.healthPercentage <= 0)
			{
				enemy.Kill();
			}

			projectile.Kill();
		}
		
	}

	void Update() 
	{

	}
};