#pragma once

#include "objects/objects.hpp"

struct _scenery_datum
{
	int32 flags;
	int16 pathfinding_policy;
	int32 lightmap_object_index;
	int32 animation_relative_object_index;
};
COMPILE_ASSERT(sizeof(_scenery_datum) == 0x10);

struct scenery_datum
{
	int32 definition_index;
	_object_datum object;
	_scenery_datum scenery;
};
COMPILE_ASSERT(sizeof(scenery_datum) == sizeof(int32) + sizeof(_object_datum) + sizeof(_scenery_datum));

