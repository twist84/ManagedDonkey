#pragma once

#include "cseries/cseries.hpp"

class c_beam_system :
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
COMPILE_ASSERT(sizeof(c_beam_system) == 0x2C);

class c_beam :
	public s_datum_header
{
	int32 m_next_sibling_index;
	int32 m_beam_gpu_index;
	int32 m_definition_block_index;
	int32 m_parent_location_index;
	int32 m_flags;
	int32 m_uncapped_length;
	int32 m_capped_length;
	int32 m_offset;
	int32 m_profile_density;
	uns32 m_random_seed_starter;
	real32 m_random_seed[2];
};
COMPILE_ASSERT(sizeof(c_beam) == 0x34);

class c_beam_location :
	public s_datum_header
{
	int32 m_next_sibling_index;
	int32 m_parent_system_index;
	int32 m_first_beam_index;
	int32 m_flags;
	real_point3d m_origin;
	real_vector3d m_direction;
	real32 m_length_cap;
};
COMPILE_ASSERT(sizeof(c_beam_location) == 0x30);

