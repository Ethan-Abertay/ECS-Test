#include "EntityManager.h"
#include "ComponentsAndSystems.h"

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

#if EM_IMPL == 2 || EM_IMPL == 3

void EntityManager::process(ECS& ecs, float DeltaTime)
{
	for (auto it = counters.begin(); it != counters.end();)
	{
		*it -= 1;

		// If timer is expired
		if (*it <= 0.f)
		{
			spawnNewEntity(ecs);		// Spawn new entity 
			it = counters.erase(it);		// Erase timer
		}
		else
			it++;	// Continue loop
	}
}

#if EM_IMPL == 3
void EntityManager::spawnMultiplier(ECS& ecs, bool init)
{
	constexpr float minMul = 0.9;
	constexpr float maxMul = 1.3;

	EntityID id = 0;
	if (init)
		id = ecs.init_CreateEntity<c::Health, c::Multiplier>();
	else
		id = ecs.createEntity<c::Health, c::Multiplier>();

	// Init component values
	ecs.getEntitysComponent<c::Health>(id)->health = 0;
	ecs.getEntitysComponent<c::Multiplier>(id)->multiplication = randRange<float>(minMul, maxMul);
}

void EntityManager::spawnSubtractor(ECS& ecs, bool init)
{
	constexpr uint minSub = 1;
	constexpr uint maxSub = 5;

	EntityID id = 0;
	if (init)
		id = ecs.init_CreateEntity<c::Health, c::Subtractor>();
	else
		id = ecs.createEntity<c::Health, c::Subtractor>();

	// Init component values
	ecs.getEntitysComponent<c::Health>(id)->health = 0;
	ecs.getEntitysComponent<c::Subtractor>(id)->subtraction = randRange<uint>(minSub, maxSub);
}
#endif

void EntityManager::addCounter()
{
	constexpr uint8_t minTime = 2.f;
	constexpr uint8_t maxTime = 5.f;

	counters.push_back(randRange(minTime, maxTime));

	//std::cout << "\t Added Counter \n";
}

#endif

void EntityManager::initSpawnEntities(ECS& ecs)
{
	auto spawnAdders = [&](int amount)
	{
		for (EntityID i = 0; i < amount; ++i)
			spawnAdder(ecs, true);
	};

#if EM_IMPL < 3

	spawnAdders(65000);

#elif EM_IMPL == 3

	auto spawnMultipliers = [&](int amount)
	{
		for (EntityID i = 0; i < amount; ++i)
			spawnMultiplier(ecs, true);	
	};
	auto spawnSubtractors = [&](int amount)
	{
		for (EntityID i = 0; i < amount; ++i)
			spawnSubtractor(ecs, true);
	};

	spawnAdders(45000);
	spawnMultipliers(5000);
	spawnSubtractors(15000);

#endif
}

void EntityManager::spawnNewEntity(ECS& ecs)
{
	//entityCounter++;
	//std::cout << entityCounter << std::endl;

#if EM_IMPL < 3

	spawnAdder(ecs, false);

#elif EM_IMPL == 3

	// 33% chance to spawn either type
	auto random = rand() % 3;
	if (random == 0)
		spawnAdder(ecs, false);
	else if (random == 1)
		spawnMultiplier(ecs, false);
	else
		spawnSubtractor(ecs, false);

#endif

	//std::cout << ecs.getNoOfEntities() << '\n';
}

template <class T>
T EntityManager::randRange(T min, T max)
{
	auto normalizedFloat = (float)(rand()) / (float)(RAND_MAX);	// Between 0.f and 1.f
	return (T)((float)min + ((float)(max - min) * normalizedFloat));
}
