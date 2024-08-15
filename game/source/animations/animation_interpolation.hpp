#pragma once

#include "cseries/cseries.hpp"

struct c_interpolator_control
{
	real m_time_remaining;
	real m_status;
	short m_interpolation_type;
	word_flags m_flags;
};
static_assert(sizeof(c_interpolator_control) == 0xC);

