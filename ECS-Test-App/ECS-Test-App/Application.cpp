#include "Application.h"
#include "ComponentsAndSystems.h"
#include "EntityManager.h"

Application::Application()
{
	// srand
	//srand(time(0));
	srand(0);

	// Initialise Entity Manager
	entityManager = new EntityManager();

	// Initialise ECS
	ecs = new ECS();

	// Initialise components
	ecs->initComponents<c::Health, c::Adder>();

#if EM_IMPL == 3

	ecs->initComponents<c::Multiplier, c::Subtractor>();

#endif

	// Initalise entities
	entityManager->initSpawnEntities(*ecs);

#if IMPL == 3

	ecs->performFullRefactor();

#endif
}

Application::~Application()
{
	if (ecs)
		delete ecs;
	ecs = 0;

	if (entityManager)
		delete entityManager;
	entityManager = 0;
}

void Application::run()
{
	// Length of time to run test in seconds
	float testTime = 15;
	unsigned int loops = 1000;

	bool bRunning = true;
	while (bRunning)
	{
		// Record start time
		auto start = Clock::now();

		// Run Game Loop
		gameLoop();

		// Record end time
		auto end = Clock::now();

		// Calculate Delta Time - Number of microseconds converted to float then divided by 1,000 twice to get delta time in seconds
		DeltaTime = (float)(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000.f / 1000.f;

		// Add delta time to record
		deltaTimes.push_back(DeltaTime);

		// Calculate fps
		FPS = 1.f / DeltaTime;

		// Decrement test time
		testTime -= DeltaTime;

		// Test if done
		//if (testTime <= 0.f)
		loops--;
		if (loops == 0)
		{
			//auto health = ecs->getEntitysComponent<c::Health>(5);
			//health->health += 1;

			// Record output
			std::ofstream file;
			file.open("Output.csv");
			for (auto& f : deltaTimes)
				file << f << std::endl;
			file.close();

			bRunning = false;
		}
	}
}

void Application::gameLoop()
{
	update();
}

void Application::update()
{
	// Handle standard systems
	

	// Handle extra systems
	eps::process(*ecs, entityManager);

#if EM_IMPL >= 2

	// Handle entity manager
	entityManager->process(*ecs, DeltaTime);

#endif
}


