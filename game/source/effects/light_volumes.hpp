#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

enum e_effect_pass;

struct c_light_volume_location :
	s_datum_header
{
	long m_next_sibling_index;
	long m_parent_system_index;
	long m_first_light_volume_index;
	long m_flags;
	real_point3d m_origin;
	real_vector3d m_direction;
};
static_assert(sizeof(c_light_volume_location) == 0x2C);

struct c_light_volume :
	s_datum_header
{
	long m_next_sibling_index;
	long m_light_volume_gpu_index;
	long m_definition_block_index;
	long m_parent_location_index;
	long m_flags;
	real m_length;
	real m_offset;
	real m_profile_density;
	real m_profile_length;
	dword m_random_seed_starter;
	real m_random_seed[2];
};
static_assert(sizeof(c_light_volume) == 0x34);

struct c_light_volume_system :
	s_datum_header
{
public:
	static void __cdecl submit_all(long user_index, e_effect_pass effect_pass);

	long m_effect_index;
	byte m_event_block_index;
	byte m_priority;
	word m_event_counter;
	long m_part_block_index;
	long m_definition_index;
	long m_first_location_index;
	word_flags m_flags;
	byte m_camera_mode;
	char m_first_person_user_mask;
	real m_age;
	real m_effect_scale_a;
	real m_effect_scale_b;
	long m_lod;
};
static_assert(sizeof(c_light_volume_system) == 0x2C);

