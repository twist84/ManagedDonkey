#pragma once

#include "cseries/cseries.hpp"

struct c_interpolator_control
{
	real32 m_time_remaining;
	real32 m_status;
	int16 m_interpolation_type;
	uns16 m_flags;
};
static_assert(sizeof(c_interpolator_control) == 0xC);

