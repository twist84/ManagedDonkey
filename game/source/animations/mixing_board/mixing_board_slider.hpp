#pragma once

#include "cseries/cseries.hpp"

struct c_mixing_board_slider
{
	int16 m_value;
	int16 m_starting_value;
	int16 m_total_seconds;
	int16 m_seconds_remaining;
	uns8 m_slider_pattern;
	uns8 m_slider_target;
	uns8 m_parent_slider_type;
	int8 m_parent_slider_index;
};
static_assert(sizeof(c_mixing_board_slider) == 0xC);

