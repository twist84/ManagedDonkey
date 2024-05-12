#pragma once

#include "ai/actors.hpp"
#include "ai/sector.hpp"
#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct s_firing_position_definition
{
	byte __data0[0x28];
};
static_assert(sizeof(s_firing_position_definition) == 0x28);

struct dynamic_firing_set_datum :
	s_datum_header
{
	short type;
	long support_object_index;
	short position_count;
	short num_members;
	c_ai_point3d position;
	vector3d up;
	vector3d forward;
	c_sector_ref __unknown34;
	bool state_valid;

	byte __pad39[0x3];

	long last_update_time;
	c_static_flags<32> initialized;
	s_firing_position_definition const firing_position[32];
	short const firing_position_types[32];
};
static_assert(sizeof(dynamic_firing_set_datum) == 0x584);

