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
				entityManager->addCounter();
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
#if EM_IMPL == 3
		auto processMultiplier = [&](EntityID i)
		{
			// Get this entity's multiplier
			auto multiplier = ecs.getEntitysComponent<c::Multiplier>(i);

			// Assume every entity has health component as that is the intention
			auto health = ecs.getEntitysComponent<c::Health>(i);

			// Apply multiplication
			health->health = uint((float)health->health * multiplier->multiplication);

			postProcess(i, health);

		};
		auto processSubtractor = [&](EntityID i)
		{
			// Get this entity's subtractor
			auto subtraction = ecs.getEntitysComponent<c::Subtractor>(i);

			// Assume every entity has health component as that is the intention
			auto health = ecs.getEntitysComponent<c::Health>(i);

			// Apply subtractor
			health->health -= subtraction->subtraction;

			// No need to process since we just wrap round thus no dying on subtraction
		};
#endif

		auto adderMask = ecs.getCompMask<c::Health, c::Adder>();

#if EM_IMPL == 3
		auto multiplerMask = ecs.getCompMask<c::Health, c::Multiplier>();
		auto subtractorMask = ecs.getCompMask<c::Health, c::Subtractor>();
#endif

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
		auto processAdderGroup = [&](ecs::EntityGroup* group)
		{
			for (int i = group->startIndex; i <= group->getEndIndex(); i++)
			{
				// Record previous count
				auto previousCount = group->noOfEntities;

				processAdder(i);

				// Detect if entity was killed
				if (previousCount != group->noOfEntities)
				{
					// Set new previous count
					previousCount = group->noOfEntities;

					// Update i for loop
					i--;
				}
			}
		}; 
#if EM_IMPL == 3

		auto processMultiplierGroup = [&](ecs::EntityGroup* group)
		{
			for (int i = group->startIndex; i <= group->getEndIndex(); i++)
				processMultiplier(i);
		};
		auto processSubtractorGroup = [&](ecs::EntityGroup* group)
		{
			for (int i = group->startIndex; i <= group->getEndIndex(); i++)
				processSubtractor(i);
		};

#endif

		// Get entity groups
		auto &entityGroups = ecs.getEntityGroups();

		// Loop through each group
		for (auto *group : entityGroups)
		{
			// If this is a group with these components
			if ((group->compMask & adderMask) == adderMask)
				processAdderGroup(group);
#if EM_IMPL == 3
			else if ((group->compMask & multiplerMask) == multiplerMask)
				processMultiplierGroup(group);
			else if ((group->compMask & subtractorMask) == subtractorMask)
				processSubtractorGroup(group);
#endif
		}

#endif
	}
};
