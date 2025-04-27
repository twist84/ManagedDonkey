#pragma once

#include "ai/actors.hpp"
#include "ai/sector.hpp"
#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct firing_position_definition
{
	c_ai_point3d position;
	short flags;
	uint16 posture_flags;
	short area_index;
	short cluster_index;
	c_sector_ref sector_ref;
	real_euler_angles2d normal;
	real32 yaw;
};
static_assert(sizeof(firing_position_definition) == 0x28);

struct dynamic_firing_set_datum :
	s_datum_header
{
	short type;
	long support_object_index;
	short position_count;
	short num_members;
	c_ai_point3d position;
	real_vector3d up;
	real_vector3d forward;
	c_sector_ref sector_ref;
	bool state_valid;
	long last_update_time;
	c_static_flags<32> initialized;
	firing_position_definition firing_positions[32];
	short firing_position_types[32];
};
static_assert(sizeof(dynamic_firing_set_datum) == 0x584);

struct dynamic_firing_set_iterator
{
	c_data_iterator<dynamic_firing_set_datum> iterator;
	long index;
};
static_assert(sizeof(dynamic_firing_set_iterator) == 0x14);

extern void __cdecl dynamic_firing_sets_dispose_from_old_map();
extern void __cdecl dynamic_firing_sets_initialize();
extern void __cdecl dynamic_firing_sets_initialize_for_new_map();

