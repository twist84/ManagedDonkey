#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

enum e_effect_pass;

struct c_contrail_system :
	s_datum_header
{
	enum e_flags
	{
		_should_live_bit = 0,
		_should_pulse_bit,
		_should_frame_advance_bit,
		_should_render_bit,
		_ever_frame_advanced_bit,

		k_flags
	};

public:
	static void __cdecl submit_all(long user_index, e_effect_pass effect_pass);

	long m_effect_index;
	byte m_event_block_index;
	byte m_priority;
	word m_event_counter;
	long m_part_block_index;
	long m_definition_index;
	long m_lightprobe_index;
	long m_first_location_index;
	c_flags<e_flags, word, k_flags> m_flags;
	byte m_camera_mode;
	char m_first_person_user_mask;
	real m_age;
	real m_effect_scale_a;
	real m_effect_scale_b;
	long m_lod;
};
static_assert(sizeof(c_contrail_system) == 0x30);

struct c_contrail :
	s_datum_header
{
	static void __cdecl render_callback(void const*, long);
	void render() const;

	enum e_flags
	{
		_should_frame_advance_bit = 0,
		_initialized_bit,

		k_flags
	};

	long m_next_sibling_index;
	long m_contrail_gpu_index;
	long m_first_profile_index;
	long m_definition_block_index;
	long m_parent_location_index;
	dword_flags m_flags;
	dword m_random_seed_starter;
	real m_random_seed[2];
	long m_profile_accumulator;
	real_point3d m_previous_profile_position;
	real m_length;
	real m_emission_rate;
	real m_profile_lifespan;
	real_point2d m_uv_offset;
};
static_assert(sizeof(c_contrail) == 0x4C);

struct c_contrail_location :
	s_datum_header
{
	enum e_flags
	{
		_should_frame_advance_bit = 0,

		k_flags
	};

	long m_next_sibling_index;
	long m_parent_system_index;
	long m_first_contrail_index;
	dword_flags m_flags;
	real_matrix4x3 m_matrix;
	vector3d m_velocity;
	real_point3d m_origin;
};
static_assert(sizeof(c_contrail_location) == 0x60);

struct c_contrail_profile :
	s_datum_header
{
	enum e_flags
	{
		_temporary_bit = 0,

		k_flags
	};

	long m_next_sibling_index;
	long m_flags;
	real m_age;
	real m_lifespan;
	word m_random_seed[4];
	real_point3d m_position;
	vector3d m_velocity;
	real m_cumulative_length;
	real_argb_color m_initial_color;
	real m_initial_alpha;
};
static_assert(sizeof(c_contrail_profile) == 0x4C);

extern bool contrail_render_enable;

