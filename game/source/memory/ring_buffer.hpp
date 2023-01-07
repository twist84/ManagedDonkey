#pragma once

#include "cseries/cseries.hpp"

struct c_ring_buffer
{
	long ring_size;
	byte __data[4];
	long element_count;
};
static_assert(sizeof(c_ring_buffer) == 0xC);
