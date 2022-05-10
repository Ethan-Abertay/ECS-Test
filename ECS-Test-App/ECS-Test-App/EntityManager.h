#pragma once
#include "../../../ECS-Library/ECS/ECS/ECS.h"

#include <vector>

/*
	Implementations:
		1.) simple, no death or variance in entities
		2.) less simple, entities can die and now be respawned which dynamically updates the entity array
		3.)	least simple, there's multiple types of entities and death is still implemented
*/
#define EM_IMPL 2

namespace sf
{
	class RenderWindow;
}

class EntityManager
{
public:
	void initSpawnEntities(ECS& ecs);
	void spawnNewEntity(ECS& ecs);

#if EM_IMPL == 2 || EM_IMPL == 3

	// Used to spawn a new entity after a length of time
	void addCounter();

	// Process counters
	void process(ECS& ecs, float DeltaTime);

#endif
protected:
	template<class T>
	T randRange(T min, T max);

	void spawnAdder(ECS& ecs, bool init);

#if EM_IMPL == 3
	void spawnMultiplier(ECS& ecs, bool init);
	void spawnSubtractor(ECS& ecs, bool init);
#endif

#if EM_IMPL >= 2

	// These implementations have death
	std::vector<uint8_t> counters;	// A vector of counters to respawn a new entity

#endif
};

