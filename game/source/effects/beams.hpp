#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

enum e_effect_pass;

struct c_beam_system :
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
static_assert(sizeof(c_beam_system) == 0x2C);

struct c_beam :
	s_datum_header
{
	long m_next_sibling_index;
	long m_beam_gpu_index;
	long m_definition_block_index;
	long m_parent_location_index;
	long m_flags;
	long m_uncapped_length;
	long m_capped_length;
	long m_offset;
	long m_profile_density;
	dword m_random_seed_starter;
	real m_random_seed[2];
};
static_assert(sizeof(c_beam) == 0x34);

struct c_beam_location :
	s_datum_header
{
	long m_next_sibling_index;
	long m_parent_system_index;
	long m_first_beam_index;
	long m_flags;
	real_point3d m_origin;
	vector3d m_direction;
	real m_length_cap;
};
static_assert(sizeof(c_beam_location) == 0x30);

