#pragma once

#include "cseries/location.hpp"
#include "math/real_math.hpp"

struct s_sound_location
{
	real_point3d position;
	real_quaternion orientation;
	real_vector3d translational_velocity;
	s_location game_location;
	byte __data2A[0x2];
};
COMPILE_ASSERT(sizeof(s_sound_location) == 0x2C);

extern void __cdecl sound_location_set_forward(s_sound_location* sound_location, const real_vector3d* forward);

