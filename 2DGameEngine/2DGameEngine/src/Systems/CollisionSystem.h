#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"


class CollisionSystem : public System 
{
public: 
	CollisionSystem() 
	{
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();
	}

	void Update(std::unique_ptr<EventBus>& eventBus) 
	{
		// AABB Colision check

		auto entities = GetSystemEntities();

		for (auto i = entities.begin(); i != entities.end(); i++) 
		{
			Entity a = *i;
			auto aTransform = a.GetComponent<TransformComponent>();
			auto aCollider = a.GetComponent<BoxColliderComponent>();

			for (auto j = i; j != entities.end(); j++) 
			{
				Entity b = *j;

				// Bypass if we are trying to test the same entity
				if (a == b) 
				{
					continue;
				}

				auto bTransform = b.GetComponent<TransformComponent>();
				auto bCollider = b.GetComponent<BoxColliderComponent>();

				// Perform the AABB collision check between entities a and b
				bool collisionHappened = CheckAABBCollision(
					aTransform.position.x + aCollider.offset.x,
					aTransform.position.y + aCollider.offset.y,
					aCollider.width,
					aCollider.heigth,

					bTransform.position.x + bCollider.offset.x,
					bTransform.position.y + bCollider.offset.y,
					bCollider.width,
					bCollider.heigth
				);

				if (collisionHappened) 
				{
					Logger::Log("Entity " + std::to_string(a.GetId()) + " is colliding with entity " + std::to_string(b.GetId()));

					// TODO: emit an event...
					eventBus->EmitEvent<CollisionEvent>(a, b);
				}
			}
		}
	}

	bool CheckAABBCollision(double aX, double aY, double aW, double aH,
		double bX, double bY, double bW, double bH)
	{
		return (
			aX < bX + bW &&
			aX + aW > bX &&
			aY < bY + bH &&
			aY + aH > bY
			);
	}
};