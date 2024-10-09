#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_particle_emitter :
	s_datum_header
{
	byte __data2[0x2];

	// 0: passed_visibility_last_frame
	// 1: failed_visibility_last_frame
	word_flags m_flags;

	byte __data6[0x2];
	long m_particle_index;
	long m_next_emitter_index;
	long m_emitter_gpu_index;
	long m_particle_location_index;
	long m_emitters_block_index;
	vector3d __vector1C;
	byte __data28[0x28];
	real __unknown50;
	real __unknown54;
	real __unknown58;
	real_matrix4x3 m_matrix;
};
static_assert(sizeof(c_particle_emitter) == 0x90);

