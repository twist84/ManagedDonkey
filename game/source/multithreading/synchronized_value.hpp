#pragma once

struct c_synchronized_long
{
public:
	long increment();
	long peek() const;
	void operator=(long Value);

protected:
	volatile long m_value;
};
static_assert(sizeof(c_synchronized_long) == 0x4);

struct c_interlocked_long
{
public:
	long peek() const;
	long set(long Value);
	long set_if_equal(long ExChange, long Comperand);

protected:
	volatile long m_value;
};
static_assert(sizeof(c_interlocked_long) == 0x4);

