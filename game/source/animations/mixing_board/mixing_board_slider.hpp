#pragma once

#include "cseries/cseries.hpp"

struct c_mixing_board_slider
{
	int16 m_value;
	int16 m_starting_value;
	int16 m_total_seconds;
	int16 m_seconds_remaining;
	uint8 m_slider_pattern;
	uint8 m_slider_target;
	uint8 m_parent_slider_type;
	char m_parent_slider_index;
};
static_assert(sizeof(c_mixing_board_slider) == 0xC);

