#pragma once

#include "cseries/cseries.hpp"
#include "memory/ring_buffer.hpp"

struct c_sliding_window
{
	bool m_initialized;
	long m_size;
	long m_leading_sequence_number;
	long m_trailing_sequence_number;
	c_ring_buffer m_window;
};
static_assert(sizeof(c_sliding_window) == 0x1C);
