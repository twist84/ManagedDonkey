#pragma once

struct c_synchronized_long
{
	volatile long m_value;
};
static_assert(sizeof(c_synchronized_long) == 0x4);
