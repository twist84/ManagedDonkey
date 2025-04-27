#pragma once

#include "cseries/cseries.hpp"

struct c_particle_location :
	s_datum_header
{
	uint8 m_random_seed;
	long m_flags;
	long m_emitter_list_head;
	long m_emitter_list_tail;
	long m_next_sibling_index;
	long m_parent_system_index;
	real_matrix4x3 m_matrix;
	real32 m_lod_amount;
	real_point3d m_bounding_sphere_center;
	real32 m_bounding_sphere_radius;
};
static_assert(sizeof(c_particle_location) == 0x60);

