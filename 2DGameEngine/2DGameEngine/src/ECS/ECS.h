#pragma once
#include <bitset>
#include <vector>


const unsigned int MAX_COMPONENTS = 32;


////////////////////////////////////////////////////////////////////////////////
// Signature
////////////////////////////////////////////////////////////////////////////////
// We use a bitset (0's and 1's) tokeep track of which components and entity has
// and also helps to keep track of which entities a system is interested in
////////////////////////////////////////////////////////////////////////////////
typedef std::bitset<MAX_COMPONENTS> Signature;

class Component 
{

};

class Entity 
{
private:
	int id;

public:
	Entity(int id) : id(id) {}; //constructor that initialise id
	int GetId() const;

};

////////////////////////////////////////////////////////////////////
// System
////////////////////////////////////////////////////////////////////
// The System processes entities that contain a specific signature
////////////////////////////////////////////////////////////////////

class System 
{
private:
	Signature componentsSignature;
	std::vector<Entity> entities;
public:
	System() = default;
	~System() = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntityFromSystem(Entity entity);
	std::vector<Entity> GetSystemEntities() const;
	Signature& GetComponentSignature() const;


};

class Registry 
{

};
