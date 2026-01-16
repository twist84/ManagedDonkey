#pragma once

#include "cseries/cseries.hpp"

class c_light_volume_location :
	public s_datum_header
{
	int32 m_next_sibling_index;
	int32 m_parent_system_index;
	int32 m_first_light_volume_index;
	int32 m_flags;
	real_point3d m_origin;
	real_vector3d m_direction;
};
COMPILE_ASSERT(sizeof(c_light_volume_location) == 0x2C);

class c_light_volume :
	public s_datum_header
{
	int32 m_next_sibling_index;
	int32 m_light_volume_gpu_index;
	int32 m_definition_block_index;
	int32 m_parent_location_index;
	int32 m_flags;
	real32 m_length;
	real32 m_offset;
	real32 m_profile_density;
	real32 m_profile_length;
	uns32 m_random_seed_starter;
	real32 m_random_seed[2];
};
COMPILE_ASSERT(sizeof(c_light_volume) == 0x34);

class c_light_volume_system :
	public s_datum_header
{
public:
	static void __cdecl submit_all(int32 user_index, e_effect_pass effect_pass);

	int32 m_effect_index;
	uns8 m_event_block_index;
	uns8 m_priority;
	uns16 m_event_counter;
	int32 m_part_block_index;
	int32 m_definition_index;
	int32 m_first_location_index;
	uns16 m_flags;
	uns8 m_camera_mode;
	int8 m_first_person_user_mask;
	real32 m_age;
	real32 m_effect_scale_a;
	real32 m_effect_scale_b;
	int32 m_lod;
};
COMPILE_ASSERT(sizeof(c_light_volume_system) == 0x2C);

