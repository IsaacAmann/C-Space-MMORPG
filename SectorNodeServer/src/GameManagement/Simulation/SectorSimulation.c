
#include "SectorSimulation.h"



//Create a new sector and store it in the address provided
void createSector(Sector* sector)
{
	//Create entity array
	sector->entities = g_array_new(FALSE, FALSE, sizeof(BaseEntity*));
	
	//Create collision handler
	//Could have written collisionhandler constructor to accept a destination pointer, did not want to rewrite it
	memcpy(&(sector->collisionHandler),createCollisionHandler(sector->entities), sizeof(CollisionHandler));
	
	
}

//Free sector memory
void deleteSector(Sector* sector)
{
	//Delete any remaining entities

	//Delete collision handler
}

//Add entity to the simulation. Can be any descendent of BaseEntity
void addEntity(BaseEntity* entity)
{
	
}

int addCollisionEntity(BaseEntity* entity)
{
	
}

int removeEntityById(int entityId)
{
	
}

BaseEntity* getEntityById(int entityId)
{
	
}
