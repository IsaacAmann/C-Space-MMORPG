#ifndef SECTOR_SIMULATION_H
#define SECTOR_SIMULATION_H

#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/math_functions.h>
#include <glib.h>

typedef struct Sector
{
	b2DynamicTree dynamicTree;
} Sector;

void createSector(Sector *sector);

#endif
