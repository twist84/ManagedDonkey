#pragma once

#include "objects/objects.hpp"

struct _scenery_datum
{
	long flags;
	short pathfinding_policy;
	long lightmap_object_index;
	long animation_relative_object_index;
};
static_assert(sizeof(_scenery_datum) == 0x10);

struct scenery_datum
{
	long definition_index;
	_object_datum object;
	_scenery_datum scenery;
};
static_assert(sizeof(scenery_datum) == sizeof(long) + sizeof(_object_datum) + sizeof(_scenery_datum));

