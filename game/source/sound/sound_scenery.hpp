#pragma once

#include "cseries/cseries.hpp"
#include "objects/objects.hpp"

struct _sound_scenery_datum
{
	int32 volume_type;
	real32 height;
	real32 override_minimum_distance;
	real32 override_maximum_distance;
	real32 override_inner_cone_angle;
	real32 override_outer_cone_angle;
	real_decibel override_outer_cone_gain_db;
};
COMPILE_ASSERT(sizeof(_sound_scenery_datum) == 0x1C);

struct sound_scenery_datum
{
	int32 definition_index;
	_object_datum object;
	_sound_scenery_datum sound_scenery;
};
COMPILE_ASSERT(sizeof(sound_scenery_datum) == sizeof(int32) + sizeof(_object_datum) + sizeof(_sound_scenery_datum));

