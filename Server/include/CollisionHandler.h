#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/math_functions.h>
#include "entities.h"
#include <stdint.h>

typedef struct CollisionHandler
{
	b2DynamicTree dynamicTree;
	uint32_t currentID;

} CollisionHandler;

CollisionHandler* createCollisionHandler();

void freeCollisionHandler(CollisionHandler* collisionHandler);

//Need way to identify entity for removal (entity ID or return index?)
//Should probably go with ID, less likely to repeat ids
int addCollisionEntity(CollisionHandler* collisionHandler, BaseEntity* entity);

int removeEntityById(CollisionHandler* collisionHandler, int entityId);

BaseEntity* getEntityById(CollisionHandler* collisionHandler, int entityId);




#endif
