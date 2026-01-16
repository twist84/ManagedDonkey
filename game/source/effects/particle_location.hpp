#pragma once

#include "cseries/cseries.hpp"

class c_particle_location :
	public s_datum_header
{
	uns8 m_random_seed;
	int32 m_flags;
	int32 m_emitter_list_head;
	int32 m_emitter_list_tail;
	int32 m_next_sibling_index;
	int32 m_parent_system_index;
	real_matrix4x3 m_matrix;
	real32 m_lod_amount;
	real_point3d m_bounding_sphere_center;
	real32 m_bounding_sphere_radius;
};
COMPILE_ASSERT(sizeof(c_particle_location) == 0x60);

