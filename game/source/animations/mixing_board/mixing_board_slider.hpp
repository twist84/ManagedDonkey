#pragma once

#include "cseries/cseries.hpp"

struct c_mixing_board_slider
{
	short m_value;
	short m_starting_value;
	short m_total_seconds;
	short m_seconds_remaining;
	uint8 m_slider_pattern;
	uint8 m_slider_target;
	uint8 m_parent_slider_type;
	char m_parent_slider_index;
};
static_assert(sizeof(c_mixing_board_slider) == 0xC);

