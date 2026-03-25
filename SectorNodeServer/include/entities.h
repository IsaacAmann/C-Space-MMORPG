#ifndef ENTITIES_H
#define ENTITIES_H

#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/math_functions.h>
#include <stdint.h>

typedef struct BaseEntity
{
	uint32_t entityType;
	//Set by collision handler, should not be set manually
	int entityId;
	b2Transform transform;
} BaseEntity;

typedef struct DynamicEntity
{
	BaseEntity baseEntity;
	b2Vec2 velocity;
} DynamicEntity;

typedef struct DestructibleEntity
{
	DynamicEntity dynamicEntity;
	int health;
} DestructibleEntity;

#endif
