#pragma once
#include "../ECS/ECS.h"
#include <sol/sol.hpp>
#include "../Components/ScriptComponent.h"
#include "../Components/TransformComponent.h"

// Declaring native C++ functions that will bind with Lua Functions

//int GetEntityPosition(Entity entity) 
//{
//    //TODO:...
//}

void SetEntityPosition(Entity entity, double x, double y) 
{
	if (entity.HasComponent<TransformComponent>()) 
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		transform.position.x = x;
		transform.position.y = y;
	}
	else 
	{
		Logger::Err("Trying to set a position to an entity that has no TransformComponent");
	}

}

class ScriptSystem : public System 
{
public:

	ScriptSystem() 
	{
		RequireComponent<ScriptComponent>();
	}

	void CreateLuaBindings(sol::state& lua) 
	{
		// Create the entity user type so lua knows what an entity is
		lua.new_usertype<Entity>
			(
				"entity",
				"get_id", &Entity::GetId,
				"destroy", &Entity::Kill,
				"has_tag", &Entity::HasTag,
				"belongs_to_group", &Entity::BelongsToGroup
			);

		lua.set_function("set_position", SetEntityPosition);

	}

	void Update(double deltaTime, int ellapsedTime) 
	{
		// Loop all entities that have a script component and invoke their Lua function
		for (auto entity : GetSystemEntities()) 
		{
			const auto script = entity.GetComponent<ScriptComponent>();
			script.func(entity, deltaTime, ellapsedTime); // Here is where the sol::function is called

		}
	}
};