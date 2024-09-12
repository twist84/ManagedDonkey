#pragma once

struct c_synchronized_long
{
public:
	c_synchronized_long();
	c_synchronized_long(long value);
	~c_synchronized_long();

	long increment();
	long peek() const;
	long set(long value);
	long add(long value);
	void operator=(long value);
	operator long() const;

protected:
	volatile long m_value;
};
static_assert(sizeof(c_synchronized_long) == 0x4);

struct c_synchronized_int64
{
public:
	__int64 increment();
	__int64 peek() const;
	void operator=(__int64 Value);

protected:
	volatile __int64 m_value;
};
static_assert(sizeof(c_synchronized_int64) == 0x8);

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
	long set(long value);
	long set_if_equal(long exchange, long comperand);
	long set_bit(long index, bool enable);
	
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

