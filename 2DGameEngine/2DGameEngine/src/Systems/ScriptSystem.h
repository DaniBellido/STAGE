#pragma once
#include "../ECS/ECS.h"
#include "../Components/ScriptComponent.h"

class ScriptSystem : public System 
{
public:

	ScriptSystem() 
	{
		RequireComponent<ScriptComponent>();
	}


	void Update() 
	{
		// Loop all entities that have a script component and invoke their Lua function
		for (auto entity : GetSystemEntities()) 
		{
			const auto script = entity.GetComponent<ScriptComponent>();
			script.func(); // Here is where the sol::function is called

		}
	}
};