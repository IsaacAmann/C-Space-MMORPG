#ifndef ENTITIES_H
#define ENTITIES_H

#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/math_functions.h>

typedef struct BaseEntity
{
	int entityType;
	b2Vec2 position;
	b2Vec2 rotation;
} BaseEntity;

typedef struct DynamicEntity
{
	BaseEntity baseEntity;
	b2Vec velocity;
} DynamicEntity;

typedef struct DestructibleEntity
{
	DynamicEntity dynamicEntity;
	int health;
} DestructibleEntity;

#endif
