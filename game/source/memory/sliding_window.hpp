#pragma once

#include "cseries/cseries.hpp"
#include "memory/ring_buffer.hpp"

struct c_sliding_window
{
	bool m_initialized;
	int32 m_size;
	int32 m_leading_sequence_number;
	int32 m_trailing_sequence_number;
	c_ring_buffer m_window;
};
static_assert(sizeof(c_sliding_window) == 0x1C);
