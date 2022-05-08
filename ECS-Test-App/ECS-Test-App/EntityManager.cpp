#include "EntityManager.h"
#include "ComponentsAndSystems.h"

#if EM_IMPL == 2 || EM_IMPL == 3

void EntityManager::process(ECS& ecs, float DeltaTime)
{
	for (auto it = timers.begin(); it != timers.end();)
	{
		*it -= DeltaTime;

		// If timer is expired
		if (*it <= 0.f)
		{
			spawnNewEntity(ecs);		// Spawn new entity 
			it = timers.erase(it);		// Erase timer
		}
		else
			it++;	// Continue loop
	}
}

void EntityManager::spawnAdder(ECS& ecs, bool init)
{
	constexpr uint minAdd = 1;
	constexpr uint maxAdd = 10;

	EntityID id = 0;
	if (init)
		id = ecs.init_CreateEntity<c::Health, c::Adder>();
	else
		id = ecs.createEntity<c::Health, c::Adder>();

	// Init component values
	ecs.getEntitysComponent<c::Health>(id)->health = 0;
	ecs.getEntitysComponent<c::Adder>(id)->addition = randRange<uint>(minAdd, maxAdd);
}

void EntityManager::addTimer()
{
	constexpr float minTime = 2.f;
	constexpr float maxTime = 5.f;

	timers.push_back(randRange(minTime, maxTime));
}

#endif

void EntityManager::initSpawnEntities(ECS& ecs)
{


#if EM_IMPL < 3

	// Spawn only regular adders
	const EntityID entityCount = 50000;
	for (EntityID i = 0; i < entityCount; ++i)
	{
		spawnAdder(ecs, true);
	}

#elif EM_IMPL == 3

#endif
}

void EntityManager::spawnNewEntity(ECS& ecs)
{
#if EM_IMPL < 3

	spawnAdder(ecs, false);

#endif
}

template <class T>
T EntityManager::randRange(T min, T max)
{
	auto normalizedFloat = (float)(rand()) / (float)(RAND_MAX);	// Between 0.f and 1.f
	return (T)((float)min + ((float)(max - min) * normalizedFloat));
}
