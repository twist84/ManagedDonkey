#pragma once

#include "cseries/cseries.hpp"

class c_contrail_definition;

class c_contrail_system :
	public s_datum_header
{
public:
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
	static void __cdecl submit_all(int32 user_index, e_effect_pass effect_pass);

	int32 m_effect_index;
	uns8 m_event_block_index;
	uns8 m_priority;
	uns16 m_event_counter;
	int32 m_part_block_index;
	int32 m_definition_index;
	int32 m_lightprobe_index;
	int32 m_first_location_index;
	c_flags<e_flags, uns16, k_flags> m_flags;
	uns8 m_camera_mode;
	int8 m_first_person_user_mask;
	real32 m_age;
	real32 m_effect_scale_a;
	real32 m_effect_scale_b;
	int32 m_lod;
};
COMPILE_ASSERT(sizeof(c_contrail_system) == 0x30);

class c_contrail_location :
	public s_datum_header
{
public:
	enum e_flags
	{
		_should_frame_advance_bit = 0,

		k_flags
	};

public:
	const c_contrail_system* get_parent_system(void) const;

public:
	int32 m_next_sibling_index;
	int32 m_parent_system_index;
	int32 m_first_contrail_index;
	uns32 m_flags;
	real_matrix4x3 m_matrix;
	real_vector3d m_velocity;
	real_point3d m_origin;
};
COMPILE_ASSERT(sizeof(c_contrail_location) == 0x60);

class c_contrail :
	public s_datum_header
{
public:
	enum e_flags
	{
		_should_frame_advance_bit = 0,
		_initialized_bit,

		k_flags
	};

public:
	static c_contrail* __cdecl get(int32 datum_index);
	const c_contrail_definition* get_definition(void) const;
	const c_contrail_location* get_parent_location(void) const;
	void render() const;
	static void __cdecl render_callback(const void* user_data, int32 user_context);

public:
	int32 m_next_sibling_index;
	int32 m_contrail_gpu_index;
	int32 m_first_profile_index;
	int32 m_definition_block_index;
	int32 m_parent_location_index;
	uns32 m_flags;
	uns32 m_random_seed_starter;
	real32 m_random_seed[2];
	int32 m_profile_accumulator;
	real_point3d m_previous_profile_position;
	real32 m_length;
	real32 m_emission_rate;
	real32 m_profile_lifespan;
	real_point2d m_uv_offset;
};
COMPILE_ASSERT(sizeof(c_contrail) == 0x4C);

class c_contrail_profile :
	public s_datum_header
{
public:
	enum e_flags
	{
		_temporary_bit = 0,

		k_flags
	};

	int32 m_next_sibling_index;
	int32 m_flags;
	real32 m_age;
	real32 m_lifespan;
	uns16 m_random_seed[4];
	real_point3d m_position;
	real_vector3d m_velocity;
	real32 m_cumulative_length;
	real_argb_color m_initial_color;
	real32 m_initial_alpha;
};
COMPILE_ASSERT(sizeof(c_contrail_profile) == 0x4C);

extern bool contrail_render_enable;

