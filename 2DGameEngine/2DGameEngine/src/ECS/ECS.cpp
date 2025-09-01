#include "ECS.h"
#include "../Logger/Logger.h"

int IComponent::nextId = 0;

int Entity::GetId() const 
{
	return id;
}

void Entity::Kill() 
{
	registry->KillEntity(*this);
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

Entity Registry::CreateEntity() 
{
	int entityId;

	if (freeIds.empty()) 
	{
		// If there's no free ids waiting to be reused
		entityId = numEntities++;

		if (entityId >= entityComponentSignatures.size())
		{
			entityComponentSignatures.resize(entityId + 1);
		}
	}
	else 
	{
		// Reuse an id from the list of removed entities

		entityId = freeIds.front();
		freeIds.pop_front();

	}

	

	Entity entity(entityId);

	entity.registry = this;

	entitiesToBeAdded.insert(entity);

	
	Logger::Log("Entity created with id = " + std::to_string(entityId));

	return entity;

}

void Registry::KillEntity(Entity entity) 
{
	entitiesToBeKilled.insert(entity);

}

void Registry::AddEntityToSystems(Entity entity)
{
	auto entityId = entity.GetId();

	const auto& entityComponentSignature = entityComponentSignatures[entityId];

	for (auto& system : systems) 
	{
		const auto& systemComponentSignature = system.second->GetComponentSignature();

		bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

		if (isInterested) 
		{
			system.second->AddEntityToSystem(entity);
		}
	}

}

void Registry::RemoveEntityFromSystems(Entity entity) 
{
	for (auto system : systems) 
	{
		system.second->RemoveEntityFromSystem(entity);
	}
}

void Registry::Update() 
{
	for (auto entity : entitiesToBeAdded) 
	{
		AddEntityToSystems(entity);
	}

	entitiesToBeAdded.clear();

	for (auto entity : entitiesToBeKilled) 
	{
		RemoveEntityFromSystems(entity);

		entityComponentSignatures[entity.GetId()].reset();

		// Make the entity ids available to be reused
		freeIds.push_back(entity.GetId());

	}

	entitiesToBeKilled.clear();
}