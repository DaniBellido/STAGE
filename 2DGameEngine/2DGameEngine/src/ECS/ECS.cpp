#include "ECS.h"

int Entity::GetId() const 
{
	return id;
}


void System::AddEntityToSystem(Entity entity) 
{
	entities.push_back(entity);

}

void System::RemoveEntityFromSystem(Entity entity) 
{
	// erase-remove idiom: Modern C++ iteration 
	entities.erase(std::remove_if(entities.begin(), entities.end(), 
		[&entity](Entity other) // Lambda Function
		{
			// Using operator overloading to compare the entity ID
			return entity == other;

		}), entities.end());

}

std::vector<Entity> System::GetSystemEntities() const 
{
	return entities;
}

const Signature& System::GetComponentSignature() const 
{
	return componentsSignature;
}