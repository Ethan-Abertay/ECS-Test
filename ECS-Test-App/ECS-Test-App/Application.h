#pragma once

#include <iostream>
#include <chrono>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <fstream>
#include <vector>

typedef std::chrono::high_resolution_clock Clock;

class Application
{
public:
	Application();
	~Application();
	void run();

private:
	// Functions
	void gameLoop();
	void update();

	// Classes
	class ECS *ecs = 0;
	class EntityManager* entityManager = 0;

	// Timer variables
	std::vector<float> deltaTimes;

	// Variables
	float DeltaTime = 0.f;
	float FPS = 0.f;
};

