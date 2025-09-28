#pragma once
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

class MovementSystem : public System 
{
public:
	MovementSystem() 
	{
		// TODO: 
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
		

	}

	void Update(double deltaTime) 
	{
		//TODO
		// Loop all the entities that the system is interested in

		for (auto entity : GetSystemEntities()) 
		{
			// Update entity position based on its velocity
			auto& transform = entity.GetComponent<TransformComponent>();
			const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

			transform.position.x += rigidbody.velocity.x * deltaTime;
			transform.position.y += rigidbody.velocity.y * deltaTime;


			bool isEntityOutsideMap = (
				transform.position.x < 0 ||
				transform.position.x > Game::mapWidth ||
				transform.position.y < 0 ||
				transform.position.y > Game::mapHeight
				);

			// Kill entities that moves outside the map
			if (isEntityOutsideMap && !entity.HasTag("player"))
			{
				entity.Kill();
			}

		}
		
	}

};