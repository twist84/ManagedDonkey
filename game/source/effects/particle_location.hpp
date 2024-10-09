#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_particle_location :
	s_datum_header
{
	byte __unknown2;
	byte __data3[0x1];

	// 0: first_person
	// 1: bounding_sphere_initialized
	// 2: passed_visibility_last_frame
	// 3: failed_visibility_last_frame
	dword_flags m_flags;

	long m_first_emitter_index;
	long m_next_emitter_index;
	long m_next_location_index;
	long m_parent_system_index;
	real_matrix4x3 m_matrix;
	real m_lod_amount;
	real_point3d m_bounding_sphere_center;
	real m_bounding_sphere_radius;
};
static_assert(sizeof(c_particle_location) == 0x60);

