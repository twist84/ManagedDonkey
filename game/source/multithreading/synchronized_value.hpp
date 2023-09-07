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
	c_interlocked_long() :
		m_value(0)
	{
	}

	c_interlocked_long(long value) :
		m_value(value)
	{
	}

	long peek() const;
	long set(long Value);
	long set_if_equal(long ExChange, long Comperand);

	operator long() const
	{
		return peek();
	}

protected:
	volatile long m_value;
};
static_assert(sizeof(c_interlocked_long) == 0x4);

template<long k_count>
struct c_synchronized_array
{
	c_synchronized_long m_storage[k_count];
};

