#pragma once

#include "EntityManager.h"

typedef uint16_t uint;

// c for components
namespace c
{
	struct Health
	{
		Health() = default;
		uint health = 0;
	};
	struct Adder
	{
		Adder() = default;
		uint addition = 0;
	};

#if EM_IMPL == 3

	// This implementations have more component variance
	struct Multiplier
	{
		Multiplier() = default;
		float multiplication = 0;
	};
	struct Subtractor
	{
		Subtractor() = default;
		uint subtraction = 0;
	};

#endif
}

// s for systems
namespace s
{

}

// eps for extra parameter systems
namespace eps
{
	static void process(ECS& ecs, EntityManager* entityManager)
	{
		auto postProcess = [&](EntityID i, c::Health* health)
		{
#if EM_IMPL >= 2

			// Detect if dead
			if (health->health >= 50)
			{
				ecs.destroyEntity(i);
				entityManager->addTimer();
			}
#endif
		};
		auto processAdder = [&](EntityID i)
		{
			// Get this entity's adder
			auto addition = ecs.getEntitysComponent<c::Adder>(i);

			// Assume every entity has health component as that is the intention
			auto health = ecs.getEntitysComponent<c::Health>(i);

			// Apply addition
			health->health += addition->addition;

			//std::cout << heatlh->health << '\n';

			postProcess(i, health);
		};
		auto processMultiplier = [&](EntityID i)
		{
			// Get this entity's adder
			auto multiplier = ecs.getEntitysComponent<c::Multiplier>(i);

			// Assume every entity has health component as that is the intention
			auto health = ecs.getEntitysComponent<c::Health>(i);

			// Apply multiplication
			health->health = 
		};
		auto processSubtractor = [&](EntityID i)
		{

		};

		auto adderMask = ecs.getCompMask<c::Health, c::Adder>();
		auto multiplerMask = ecs.getCompMask<c::Health, c::Multiplier>();
		auto subtractorMask = ecs.getCompMask<c::Health, c::Subtractor>();

#if IMPL == 1 || IMPL == 2

		// Loop through entities
		for (EntityID i = 0; i < ecs.getNoOfEntities(); i++)
		{
			// Test if correct type of entity
			if (ecs.entityHasComponents(i, adderMask))
				processAdder(i);

#if EM_IMPL == 3

			// Test if correct type of entity
			else if (ecs.entityHasComponents(i, multiplerMask))
				processMultiplier(i);
			else if (ecs.entityHasComponents(i, subtractorMask))
				processSubtractor(i);

#endif
		}

#elif IMPL == 3

		// Requires slightly more complex info to loop but it is more efficient. 
		auto processGroup = [&](ecs::EntityGroup* group)
		{
			for (int i = group->startIndex; i <= group->getEndIndex(); i++)
				processAdder(i);
		};


		// Get entity groups
		auto &entityGroups = ecs.getEntityGroups();

		// Loop through each group
		for (auto *group : entityGroups)
		{
			// If this is a group with these components
			if ((group->compMask & compMask) == compMask)
				processGroup(group);
		}

#endif
	}
};
