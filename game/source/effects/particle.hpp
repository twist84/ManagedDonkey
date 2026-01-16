#pragma once

#include "cseries/cseries.hpp"

class c_particle :
	public s_datum_header
{
	struct s_property_values
	{
		real32 m_manual_rotation;
		real32 m_animation_rate;
		real_vector3d m_self_acceleration;
	};
	COMPILE_ASSERT(sizeof(s_property_values) == 0x14);

	uns16 m_flags : 13;
	uns16 m_collision_countdown : 3;
	int32 m_next_particle;
	int32 m_parent_emitter_index;
	unsigned int m_gpu_address;
	real32 m_age;
	real32 m_inverse_lifespan;
	real32 m_emission_time;
	uns16 m_random_seed[8];
	real_point3d m_position;
	real_vector3d m_velocity;
	real32 m_angular_velocity;
	real32 m_physical_rotation;
	real_argb_color m_initial_color;
	real_vector3d m_axis;
	s_property_values m_property_values;
};
COMPILE_ASSERT(sizeof(c_particle) == 0x7C);

