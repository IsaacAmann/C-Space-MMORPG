#include "CollisionHandler.h"
#include <stdlib.h>

CollisionHandler* createCollisionHandler(GArray* entities)
{
	CollisionHandler* collisionHandler = (CollisionHandler*) malloc(sizeof(CollisionHandler));
	
	collisionHandler->entities = entities;
	collisionHandler->currentID = 0;
	
	collisionHandler->dynamicTree = b2DynamicTree_Create();
	
	return collisionHandler;
}

void freeCollisionHandler(CollisionHandler* collisionHandler)
{
	b2DynamicTree_Destroy(&(collisionHandler->dynamicTree));
	
	free(collisionHandler);
}


