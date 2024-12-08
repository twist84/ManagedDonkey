#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_particle :
	s_datum_header
{
	struct s_property_values
	{
		real m_manual_rotation;
		real m_animation_rate;
		real_vector3d m_self_acceleration;
	};
	static_assert(sizeof(s_property_values) == 0x14);

	word_flags m_flags : 13;
	word m_collision_countdown : 3;
	long m_next_particle;
	long m_parent_emitter_index;
	unsigned int m_gpu_address;
	real m_age;
	real m_inverse_lifespan;
	real m_emission_time;
	word m_random_seed[8];
	real_point3d m_position;
	real_vector3d m_velocity;
	real m_angular_velocity;
	real m_physical_rotation;
	real_argb_color m_initial_color;
	real_vector3d m_axis;
	s_property_values m_property_values;
};
static_assert(sizeof(c_particle) == 0x7C);

