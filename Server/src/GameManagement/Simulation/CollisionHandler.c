#include "CollisionHandler.h"
#include <stdlib.h>

CollisionHandler* createCollisionHandler()
{
	CollisionHandler* collisionHandler = (CollisionHandler*) malloc(sizeof(CollisionHandler));
	
	collisionHandler->currentID = 0;
	
	collisionHandler->dynamicTree = b2DynamicTree_Create();
	
	return collisionHandler;
}

void freeCollisionHandler(CollisionHandler* collisionHandler)
{
	b2DynamicTree_Destroy(&(collisionHandler->dynamicTree));
	
	
	free(collisionHandler);
}


int addCollisionEntity(CollisionHandler* collisionHandler, BaseEntity* entity)
{
	
}

int removeEntityById(CollisionHandler* collisionHandler, int entityId)
{
	
}

BaseEntity* getEntityById(CollisionHandler* collisionHandler, int entityId)
{
	
}
