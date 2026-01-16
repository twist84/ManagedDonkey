#pragma once

#include "cseries/cseries.hpp"

class c_interpolator_control
{
public:
	real32 m_time_remaining;
	real32 m_status;
	int16 m_interpolation_type;
	uns16 m_flags;
};
COMPILE_ASSERT(sizeof(c_interpolator_control) == 0xC);

