#pragma once

#include "cseries/cseries.hpp"

struct c_particle_emitter :
	s_datum_header
{
	struct s_property_values
	{
		real_point3d m_translational_offset;
		real_vector3d m_direction;
		real32 m_emission_radius;
		real32 m_emission_angle;
		real32 m_particle_axis_angle;
		real32 m_particle_lifespan;
		real32 m_particle_speed;
		real32 m_particle_angular_velocity;
		real32 m_emission_rate;
		real32 m_starting_count;
		real32 m_max_count;
	};
	static_assert(sizeof(s_property_values) == 0x3C);

	uint16 m_particle_cpu_count;
	uint16 m_flags;
	long m_particle_list_head;
	long m_next_sibling_index;
	long m_emitter_gpu_index;
	long m_parent_location_index;
	long m_definition_block_index;
	s_property_values m_property_values;
	real32 m_particle_accumulator;
	real_matrix4x3 m_matrix;
};
static_assert(sizeof(c_particle_emitter) == 0x90);

