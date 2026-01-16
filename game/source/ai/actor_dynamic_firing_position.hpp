#pragma once

#include "ai/actors.hpp"
#include "ai/sector.hpp"
#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct firing_position_definition
{
	c_ai_point3d position;
	int16 flags;
	uns16 posture_flags;
	int16 area_index;
	int16 cluster_index;
	c_sector_ref sector_ref;
	real_euler_angles2d normal;
	real32 yaw;
};
COMPILE_ASSERT(sizeof(firing_position_definition) == 0x28);

struct dynamic_firing_set_datum :
	s_datum_header
{
	int16 type;
	int32 support_object_index;
	int16 position_count;
	int16 num_members;
	c_ai_point3d position;
	real_vector3d up;
	real_vector3d forward;
	c_sector_ref sector_ref;
	bool state_valid;
	int32 last_update_time;
	c_static_flags<32> initialized;
	firing_position_definition firing_positions[32];
	int16 firing_position_types[32];
};
COMPILE_ASSERT(sizeof(dynamic_firing_set_datum) == 0x584);

struct dynamic_firing_set_iterator
{
	c_data_iterator<dynamic_firing_set_datum> iterator;
	int32 index;
};
COMPILE_ASSERT(sizeof(dynamic_firing_set_iterator) == 0x14);

extern void __cdecl dynamic_firing_sets_dispose_from_old_map();
extern void __cdecl dynamic_firing_sets_initialize();
extern void __cdecl dynamic_firing_sets_initialize_for_new_map();

