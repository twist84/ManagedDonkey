#pragma once

#include "cseries/cseries.hpp"
#include "objects/objects.hpp"

struct _sound_scenery_datum
{
	long volume_type;
	real height;
	real override_minimum_distance;
	real override_maximum_distance;
	real override_inner_cone_angle;
	real override_outer_cone_angle;
	real_decibel override_outer_cone_gain_db;
};
static_assert(sizeof(_sound_scenery_datum) == 0x1C);

struct sound_scenery_datum
{
	long definition_index;
	_object_datum object;
	_sound_scenery_datum sound_scenery;
};
static_assert(sizeof(sound_scenery_datum) == sizeof(long) + sizeof(_object_datum) + sizeof(_sound_scenery_datum));

