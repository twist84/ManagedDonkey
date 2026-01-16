#pragma once

#include "cseries/cseries.hpp"

class c_patchy_fog
{
public:
	static void __cdecl frame_advance_all(real32 seconds_elapsed);

	real_point3d m_last_eye_position;
	real_vector3d m_last_forward;
	real_vector3d m_last_up;
	real32 m_roll;
	real32 m_closest_fog_z;
	real32 m_distance_to_first_sheet;
	int32 m_starting_sheet_attribute_index;
	real32 m_lateral_offsets[100];
	real32 m_vertical_offsets[100];
};
COMPILE_ASSERT(sizeof(c_patchy_fog) == 0x354);

