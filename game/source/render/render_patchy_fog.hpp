#pragma once

#include "cseries/cseries.hpp"

struct c_patchy_fog
{
	static void __cdecl frame_advance_all(real seconds_elapsed);

	real_point3d m_last_eye_position;
	real_vector3d m_last_forward;
	real_vector3d m_last_up;
	real m_roll;
	real m_closest_fog_z;
	real m_distance_to_first_sheet;
	long m_starting_sheet_attribute_index;
	real m_lateral_offsets[100];
	real m_vertical_offsets[100];
};
static_assert(sizeof(c_patchy_fog) == 0x354);

