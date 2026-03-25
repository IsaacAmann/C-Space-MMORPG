#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/math_functions.h>
#include "entities.h"
#include <stdint.h>
#include <glib.h>


typedef struct CollisionHandler
{
	b2DynamicTree dynamicTree;
	uint32_t currentID;
	//will probably just be set to the sector's entity list, no reason for there to be separate arrays at the moment
	GArray* entities;

} CollisionHandler;

CollisionHandler* createCollisionHandler(GArray* entities);

void freeCollisionHandler(CollisionHandler* collisionHandler);






#endif
