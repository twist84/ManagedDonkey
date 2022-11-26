#pragma once

struct c_synchronized_long
{
	volatile long m_value;
};
static_assert(sizeof(c_synchronized_long) == 0x4);

struct c_interlocked_long
{
public:
	long set_if_equal(long ExChange, long Comperand);

protected:
	volatile long m_value;
};
static_assert(sizeof(c_interlocked_long) == 0x4);
