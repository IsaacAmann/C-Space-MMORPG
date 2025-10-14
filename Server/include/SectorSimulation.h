#ifndef SECTOR_SIMULATION_H
#define SECTOR_SIMULATION_H

#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/math_functions.h>
#include <glib.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct Sector
{
	b2DynamicTree dynamicTree;
	pthread_t simulationLoopThread;
	int isRunning;
	
} Sector;

//Create a new sector and store it in the address provided
void createSector(Sector* sector);

//Free sector memory
void deleteSector(Sector* sector);

//Add entity to the simulation. Can be any descendent of BaseEntity
void addEntity(BaseEntity* entity);

//Contains statements that should be run each step of the simulation
void* playerConnectionThreadRun(void *args)






#endif
